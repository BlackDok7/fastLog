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
    #ifdef __STDC_LIB_EXT1__
        vsnprintf_s(log_entry, MAX_LOG_MESSAGE, MAX_LOG_MESSAGE - 1, format, args);
    #else
        vsnprintf(log_entry, MAX_LOG_MESSAGE - 1, format, args);
    #endif

    va_end(args);

    log_backend_write(log_entry);
    free(log_entry);
}

void set_log_level(log_level_t level) {
    global_log_level = level;
}
