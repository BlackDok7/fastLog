#include "fastlog_backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <unistd.h>

log_backend_t global_log_backend = LOG_BACKEND_STDOUT;
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
    }
}

void log_backend_write(const char *log_entry) {
    switch (global_log_backend) {
        case LOG_BACKEND_STDOUT:
            printf("%s\n", log_entry);
            break;
        case LOG_BACKEND_SYSLOG:
            syslog(LOG_INFO, "%s", log_entry);
            break;
        case LOG_BACKEND_FILE:
            if (log_file) {
                fprintf(log_file, "%s\n", log_entry);
                fflush(log_file);
            }
            break;
        case LOG_BACKEND_UDP:
            if (udp_socket != -1) {
                sendto(udp_socket, log_entry, strlen(log_entry), 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
            }
            break;
        case LOG_BACKEND_UART:
            // UART handling (not implemented in this part)
            break;
    }
}
