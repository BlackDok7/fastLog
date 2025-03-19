#include "fastlog.h"
#include "fastlog_backend.h"
#include "fastlog_config.h"
#include "fastlog_utils.h"
#include <time.h>
#include <sys/time.h>

log_level_t global_log_level = LOG_LEVEL_INFO;

void log_message(log_level_t level, const char *format, ...) {
    if (level < global_log_level) return;

    char timestamp[32];
    get_current_timestamp(timestamp, sizeof(timestamp));

    char *log_entry = (char *)malloc(MAX_LOG_MESSAGE);
    if (!log_entry) return;

    va_list args;
    va_start(args, format);
    vsnprintf(log_entry, MAX_LOG_MESSAGE, format, args);
    va_end(args);

    log_backend_write(log_entry);
    free(log_entry);
}

void set_log_level(log_level_t level) {
    global_log_level = level;
}
