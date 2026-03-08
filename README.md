# hope-logger

Minimal asynchronous C++ logger with stream abstraction, message formatting, and optional background flushing.

## Why this library

`hope-logger` provides:

- simple log API through streaming operators
- message timestamp and level prefix formatting
- pluggable output streams (`console`, `file`, `multi-stream`)
- optional background flush thread for asynchronous writes
- lightweight dependency-free implementation (STL only)

## Requirements

- C++17 or newer
- CMake 3.11+ (root project uses 3.11, sample uses 3.20)

## Build

```bash
git clone https://github.com/glensand/hope-logger.git
cd hope-logger
cmake -S . -B build
cmake --build build -j
```

Sample executable is produced in `build/bin/sample`.

## Quick start

```cpp
#include "hope_logger/logger.h"
#include "hope_logger/log_helper.h"
#include "hope_logger/ostream.h"

int main() {
    auto* stream = hope::log::create_console_stream();
    auto* logger = new hope::log::logger(*stream);

    HOPE_INTERIOR_LOG(hope::log::log_level::info, *logger) << "Hello logger";
    HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Something happened";

    delete logger;
    delete stream;
}
```

Output format example:

```text
[08-03-2026 14:10:22] (Info) Hello logger
[08-03-2026 14:10:22] (Error) Something happened
```

## Usage examples

### 1) Log to file

```cpp
auto* file_stream = hope::log::create_file_stream("logs/app.log", 16); // 16 MB rotation limit
auto* logger = new hope::log::logger(*file_stream);

HOPE_INTERIOR_LOG(hope::log::log_level::info, *logger) << "File logging enabled";

delete logger;
delete file_stream;
```

Notes:

- file names are prefixed with a timestamp when created
- parent directories are created automatically
- once limit is reached, stream rotates to a new timestamped file

### 2) Log to console + file simultaneously

```cpp
auto* stream = hope::log::create_multy_stream({
    hope::log::create_console_stream(),
    hope::log::create_file_stream("logs/app.log")
});
auto* logger = new hope::log::logger(*stream);

HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Visible in both outputs";

delete logger;
delete stream; // deletes child streams internally
```

### 3) Structured value hint with `HOPE_VAL`

`HOPE_VAL(x)` wraps value as `[x]` in message output.

```cpp
int user_id = 42;
double load = 0.73;

HOPE_INTERIOR_LOG(hope::log::log_level::info, *logger)
    << "User id:" << HOPE_VAL(user_id) << "cpu load:" << HOPE_VAL(load);
```

Possible output:

```text
[08-03-2026 14:12:01] (Info) User id: [42] cpu load: [0.73]
```

### 4) Disable background flush thread

By default logger may use a dedicated flush thread. You can disable it:

```cpp
auto* stream = hope::log::create_console_stream();
auto* logger = new hope::log::logger(
    *stream,
    std::thread::hardware_concurrency(),
    false // use_flush_thread
);

HOPE_INTERIOR_LOG(hope::log::log_level::info, *logger) << "Synchronous flush mode";

delete logger;
delete stream;
```

## Log levels

Current levels:

- `hope::log::log_level::info`
- `hope::log::log_level::error`

You can configure logger threshold:

```cpp
logger->set_log_level(hope::log::log_level::info);
auto level = logger->get_log_level();
```

## Include and link in your project

When using this repository as a subdirectory:

```cmake
add_subdirectory(path/to/hope-logger/lib)
target_link_libraries(your_target PRIVATE hope_logger)
target_include_directories(your_target PRIVATE path/to/hope-logger/lib)
```

Or use repository root and link the library target from there.

## License

MIT, see `LICENSE`.

## Reference

This project is part of the Hope ecosystem and follows ideas and style from the main Hope repository:

- [glensand/hope](https://github.com/glensand/hope)