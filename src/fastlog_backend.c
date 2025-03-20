#include "fastlog_backend.h"
#include "fastlog_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>  // For thread safety (mutex)

#define MAX_LOG_SIZE 1048576  // 1MB max log file size before rotation
#define MAX_BACKENDS 3        // Allow up to 3 simultaneous log backends

// Global mutex for thread-safe logging
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static log_backend_t active_backends[MAX_BACKENDS];
static int backend_count = 0;

static FILE *log_file = NULL;
static char log_filename[256] = "logfile.txt";

static int udp_socket = -1;
static struct sockaddr_in udp_addr;

#ifdef ENABLE_LOG_ROTATION
// Rotate log file if it exceeds the maximum allowed size
void rotate_log_file() {
    pthread_mutex_lock(&log_mutex);  // Ensure thread safety

    if (!log_file) {
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    fclose(log_file);

    char backup_filename[260];
    snprintf(backup_filename, sizeof(backup_filename), "%s.old", log_filename);
    rename(log_filename, backup_filename);

    log_file = fopen(log_filename, "w");

    pthread_mutex_unlock(&log_mutex);
}

// Check log file size and rotate if necessary
void check_log_size() {
    struct stat st;
    if (stat(log_filename, &st) == 0 && st.st_size > MAX_LOG_SIZE) {
        rotate_log_file();
    }
}
#endif

// Add a logging backend (up to `MAX_BACKENDS` can be used simultaneously)
void set_log_backend(log_backend_t backend, const char *param) {
    pthread_mutex_lock(&log_mutex);  // Ensure thread safety

    if (backend_count < MAX_BACKENDS) {
        active_backends[backend_count] = backend;
        backend_count++;
    }

    // File backend setup
    if (backend == LOG_BACKEND_FILE && param) {
        strncpy(log_filename, param, sizeof(log_filename) - 1);
        log_file = fopen(log_filename, "a");
    }

    // UDP backend setup
    if (backend == LOG_BACKEND_UDP && param) {
        if (udp_socket != -1) close(udp_socket);
        udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_port = htons(514);
        udp_addr.sin_addr.s_addr = inet_addr(param);
    }

    pthread_mutex_unlock(&log_mutex);
}

// Write log messages to all active backends
void log_backend_write(const char *log_entry) {
    pthread_mutex_lock(&log_mutex);  // Lock before writing

    #ifdef ENABLE_LOG_ROTATION
    check_log_size();
    #endif

    for (int i = 0; i < backend_count; i++) {
        switch (active_backends[i]) {
            case LOG_BACKEND_STDOUT:
                printf("%s\n", log_entry);
                fflush(stdout);
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
                    sendto(udp_socket, log_entry, strlen(log_entry), 0, 
                           (struct sockaddr *)&udp_addr, sizeof(udp_addr));
                }
                break;
            case LOG_BACKEND_UART:
                // UART logging implementation (future feature)
                break;
        }
    }

    pthread_mutex_unlock(&log_mutex);  // Unlock after writing
}
