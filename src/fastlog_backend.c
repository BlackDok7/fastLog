#include "fastlog_backend.h"
#include "fastlog_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_LOG_SIZE 1048576  // 1MB max log file size before rotation

log_backend_t global_log_backend = LOG_BACKEND_STDOUT;
static FILE *log_file = NULL;
static char log_filename[256] = "logfile.txt";

void rotate_log_file() {
    if (!log_file) return;

    fclose(log_file);

    char backup_filename[260];
    snprintf(backup_filename, sizeof(backup_filename), "%s.old", log_filename);
    rename(log_filename, backup_filename);

    log_file = fopen(log_filename, "w");
}

void check_log_size() {
    struct stat st;
    if (stat(log_filename, &st) == 0 && st.st_size > MAX_LOG_SIZE) {
        rotate_log_file();
    }
}

void set_log_backend(log_backend_t backend, const char *param) {
    global_log_backend = backend;
    
    if (backend == LOG_BACKEND_FILE && param) {
        strncpy(log_filename, param, sizeof(log_filename) - 1);
        log_file = fopen(log_filename, "a");
    }
}

#ifdef ENABLE_LOG_ROTATION
void check_log_size();
#endif

void log_backend_write(const char *log_entry) {
    #ifdef ENABLE_LOG_ROTATION
    check_log_size();
    #endif

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
        #ifdef ENABLE_UDP_BACKEND
        case LOG_BACKEND_UDP:
            // UDP logging implementation
            break;
        #endif
        #ifdef ENABLE_UART_BACKEND
        case LOG_BACKEND_UART:
            // UART logging implementation
            break;
        #endif
    }
}
