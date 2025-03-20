#include "fastlog.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  // For sleep

void* log_thread(void* arg) {
    for (int i = 0; i < 5; i++) {
        log_message(LOG_LEVEL_INFO, "Thread %d: Log message %d", (int)arg, i);
        usleep(50000);  // Simulate work
    }
    return NULL;
}

int main() {
    set_log_level(LOG_LEVEL_INFO);

    // Enable logging to multiple backends
    set_log_backend(LOG_BACKEND_STDOUT, NULL);
    set_log_backend(LOG_BACKEND_FILE, "logfile.txt");

    // Create threads for concurrent logging
    pthread_t t1, t2;
    pthread_create(&t1, NULL, log_thread, (void*)1);
    pthread_create(&t2, NULL, log_thread, (void*)2);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
