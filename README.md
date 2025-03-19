# Embedded FastLog
A lightweight, high-performance logging framework for embedded Linux and real-time applications.

## Features
✅ Low-Latency Logging – Uses a lock-free ring buffer for real-time efficiency.  
✅ Multi-Backend Support – Log to stdout, syslog, files, UDP, or UART.  
✅ Log Rotation – Automatically rotates logs when they exceed a defined size.  
✅ Compile-Time Configuration – Enable/disable features for flexibility.  
✅ Fine-Grained Log Levels – Supports INFO, DEBUG, and ERROR.  
✅ Cross-Platform – Works on Linux, QNX, OpenWrt, and embedded systems.  

## Installation
Clone the repository:
git clone https://github.com/BlackDok7/fastLog.git
cd fastLog

Compile the static library:
make

Install the library system-wide (optional):
sudo make install

Uninstall:
sudo make uninstall

## Usage
### 1️ Include the Header
In your project, include `fastlog.h`:
#include "fastlog.h"

### 2️ Set Logging Level
Choose the minimum log level:
set_log_level(LOG_LEVEL_DEBUG);

### 3️ Set Logging Backend
Specify where logs should be written:
set_log_backend(LOG_BACKEND_FILE, "logfile.txt");  // File logging
set_log_backend(LOG_BACKEND_STDOUT, NULL);        // Console output
set_log_backend(LOG_BACKEND_UDP, "192.168.1.10"); // UDP server
set_log_backend(LOG_BACKEND_UART, "/dev/ttyS0");  // UART

### 4 Log Messages
Log events in your application:
log_message(LOG_LEVEL_INFO, "System initialized.");
log_message(LOG_LEVEL_DEBUG, "Debug mode enabled.");
log_message(LOG_LEVEL_ERROR, "Critical error encountered!");


## Configuration
Modify `include/fastlog_config.h` for compile-time customization.

| Setting                 | Default | Description                     |
|-------------------------|---------|---------------------------------|
| `ENABLE_LOG_ROTATION`   | `1`     | Enable automatic log rotation  |
| `ENABLE_UDP_BACKEND`    | `1`     | Enable UDP logging backend     |
| `ENABLE_UART_BACKEND`   | `0`     | Enable UART logging backend    |
| `MAX_LOG_MESSAGE`       | `128`   | Max log message size in bytes  |

To disable log rotation, edit `fastlog_config.h`:
#define ENABLE_LOG_ROTATION 0
Or compile with:
gcc -DENABLE_LOG_ROTATION=0 -o myapp src/*.c

## Example Program
A simple example using Embedded FastLog:

#include "fastlog.h"

int main() {
    set_log_level(LOG_LEVEL_DEBUG);
    set_log_backend(LOG_BACKEND_FILE, "logfile.txt");

    log_message(LOG_LEVEL_INFO, "System initialized.");
    log_message(LOG_LEVEL_DEBUG, "Debugging mode enabled.");
    log_message(LOG_LEVEL_ERROR, "Critical error encountered.");

    return 0;
}

Compile and run:
gcc -o example examples/main.c -L. -lfastlog
./example

## Compiling from Source
To compile the project, run:
make

To clean up:
make clean

## File Structure
/embedded-fastlog
├── include/
│   ├── fastlog.h         # Public API
│   ├── fastlog_backend.h # Backend functions
│   ├── fastlog_config.h  # Compile-time config
│   ├── fastlog_utils.h   # Utility functions
├── src/
│   ├── fastlog.c         # Core logging logic
│   ├── fastlog_backend.c # Backend implementations
│   ├── fastlog_utils.c   # Utility functions
├── examples/
│   ├── main.c            # Example usage
├── LICENSE
├── README.md
├── Makefile

## License
This project is licensed under GPL-3.0. Commercial licenses are available for large enterprises.

📩 Contact me at `dima.prutean7@gmail.com` for licensing inquiries.
