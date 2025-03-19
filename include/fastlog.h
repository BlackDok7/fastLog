#ifndef FASTLOG_H
#define FASTLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdatomic.h>

// Logging levels
typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_ERROR
} log_level_t;

// Logging backends
typedef enum {
    LOG_BACKEND_STDOUT,
    LOG_BACKEND_SYSLOG,
    LOG_BACKEND_FILE,
    LOG_BACKEND_UDP,
    LOG_BACKEND_UART
} log_backend_t;

// Logging API
void set_log_level(log_level_t level);
void set_log_backend(log_backend_t backend, const char *param);
void log_message(log_level_t level, const char *format, ...);

#endif
