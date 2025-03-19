/*
 * Embedded FastLog - A Lightweight Logging Framework for Linux
 * Author: Dumitru Prutean
 * Description: A high-performance, low-latency logging framework for embedded Linux & real-time applications.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <string.h>
 #include <pthread.h>
 #include <time.h>
 #include <sys/time.h>
 #include <syslog.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <termios.h>
 #include <stdatomic.h>
 
 #define LOG_BUFFER_SIZE 256  // Reduced buffer size for embedded systems
 #define MAX_LOG_MESSAGE 128  // Reduce log message size to save memory
 
 typedef enum {
     LOG_LEVEL_INFO,
     LOG_LEVEL_DEBUG,
     LOG_LEVEL_ERROR
 } log_level_t;
 
 typedef enum {
     LOG_BACKEND_STDOUT,
     LOG_BACKEND_SYSLOG,
     LOG_BACKEND_FILE,
     LOG_BACKEND_UDP,
     LOG_BACKEND_UART
 } log_backend_t;
 
 typedef struct {
     char *buffer[LOG_BUFFER_SIZE];
     atomic_int head;
     atomic_int tail;
 } log_ring_buffer_t;
 
 static log_ring_buffer_t log_buffer = { .head = 0, .tail = 0 };
 static log_level_t global_log_level = LOG_LEVEL_INFO;
 static log_backend_t global_log_backend = LOG_BACKEND_STDOUT;
 static FILE *log_file = NULL;
 static int udp_socket = -1;
 static struct sockaddr_in udp_addr;
 static int uart_fd = -1;
 
 void set_log_backend(log_backend_t backend, const char *param) {
     global_log_backend = backend;
     
     if (backend == LOG_BACKEND_FILE && param) {
         if (log_file) fclose(log_file);
         log_file = fopen(param, "a");
     } else if (backend == LOG_BACKEND_UDP && param) {
         if (udp_socket != -1) close(udp_socket);
         udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
         udp_addr.sin_family = AF_INET;
         udp_addr.sin_port = htons(514);
         udp_addr.sin_addr.s_addr = inet_addr(param);
     } else if (backend == LOG_BACKEND_UART && param) {
         if (uart_fd != -1) close(uart_fd);
         uart_fd = open(param, O_RDWR | O_NOCTTY | O_SYNC);
         struct termios tty;
         if (tcgetattr(uart_fd, &tty) == 0) {
             cfsetospeed(&tty, B115200);
             cfsetispeed(&tty, B115200);
             tcsetattr(uart_fd, TCSANOW, &tty);
         }
     }
 }
 
 void log_message(log_level_t level, const char *format, ...) {
     if (level < global_log_level) return;
     
     struct timeval tv;
     gettimeofday(&tv, NULL);
     struct tm *tm_info = localtime(&tv.tv_sec);
     char timestamp[32];  // Reduce timestamp buffer size
     strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);
     
     char *log_entry = (char *)malloc(MAX_LOG_MESSAGE);
     if (!log_entry) return;
     
     va_list args;
     va_start(args, format);
     vsnprintf(log_entry, MAX_LOG_MESSAGE, format, args);
     va_end(args);
     
     int head = atomic_fetch_add(&log_buffer.head, 1) % LOG_BUFFER_SIZE;
     log_buffer.buffer[head] = log_entry;
     
     switch (global_log_backend) {
         case LOG_BACKEND_STDOUT:
             printf("[%s] %s\n", timestamp, log_entry);
             break;
         case LOG_BACKEND_SYSLOG:
             syslog(LOG_INFO, "%s", log_entry);
             break;
         case LOG_BACKEND_FILE:
             if (log_file) {
                 fprintf(log_file, "[%s] %s\n", timestamp, log_entry);
                 fflush(log_file);
             }
             break;
         case LOG_BACKEND_UDP:
             if (udp_socket != -1) {
                 sendto(udp_socket, log_entry, strlen(log_entry), 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
             }
             break;
         case LOG_BACKEND_UART:
             if (uart_fd != -1) {
                 write(uart_fd, log_entry, strlen(log_entry));
                 write(uart_fd, "\n", 1);
             }
             break;
     }
     free(log_entry);
 }
 
 void set_log_level(log_level_t level) {
     global_log_level = level;
 }
 
 int main() {
     set_log_level(LOG_LEVEL_DEBUG);
     set_log_backend(LOG_BACKEND_FILE, "logfile.txt");
     log_message(LOG_LEVEL_INFO, "System initialized.");
     log_message(LOG_LEVEL_DEBUG, "Debugging mode enabled.");
     log_message(LOG_LEVEL_ERROR, "Critical error encountered.");
     return 0;
 }
 