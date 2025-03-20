#include "fastlog.h"
#include <stdio.h>
#include <unistd.h>  // For sleep

int main() {
    set_log_level(LOG_LEVEL_DEBUG);

    // Log to both file and console
    set_log_backend(LOG_BACKEND_STDOUT, NULL);
    set_log_backend(LOG_BACKEND_FILE, "logfile.txt");

    log_message(LOG_LEVEL_INFO, "System initialized.");
    log_message(LOG_LEVEL_DEBUG, "Debugging mode enabled.");
    log_message(LOG_LEVEL_ERROR, "Critical error encountered.");

    // Force flush logs and prevent premature exit
    fflush(stdout);
    sleep(1);  // Wait for logs to be written

    return 0;
}
