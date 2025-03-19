#ifndef FASTLOG_BACKEND_H
#define FASTLOG_BACKEND_H

#include "fastlog.h"

// Backend handling functions
void rotate_log_file();
void check_log_size();
void set_log_backend(log_backend_t backend, const char *param);
void log_backend_write(const char *log_entry);

#endif
