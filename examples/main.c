#include "fastlog.h"
#include <stdio.h>
#include <unistd.h>  // For sleep

int main() {
    set_log_level(LOG_LEVEL_INFO);

    // Enable logging to multiple backends
    set_log_backend(LOG_BACKEND_STDOUT, NULL);
    set_log_backend(LOG_BACKEND_FILE, "logfile.txt");
    set_log_backend(LOG_BACKEND_UDP, "192.168.1.10");  // UDP logging

    log_message(LOG_LEVEL_INFO, "System initialized.");
    log_message(LOG_LEVEL_DEBUG, "Debugging mode enabled.");
    log_message(LOG_LEVEL_ERROR, "Critical error encountered!");

    // Flush logs and prevent premature exit
    fflush(stdout);
    sleep(1);

    return 0;
}
