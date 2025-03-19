#include "fastlog.h"

int main() {
    set_log_level(LOG_LEVEL_DEBUG);
    set_log_backend(LOG_BACKEND_FILE, "logfile.txt");

    log_message(LOG_LEVEL_INFO, "System initialized.");
    log_message(LOG_LEVEL_DEBUG, "Debugging mode enabled.");
    log_message(LOG_LEVEL_ERROR, "Critical error encountered.");

    return 0;
}
