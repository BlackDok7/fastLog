#include "fastlog_utils.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void get_current_timestamp(char *buffer, size_t size) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm_info = localtime(&tv.tv_sec);
    strftime(buffer, size, "%H:%M:%S", tm_info);
}
