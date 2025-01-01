#include "hope_logger/logger.h"
#include "hope_logger/log_helper.h"
#include "hope_logger/ostream.h"

#include <iostream>

int main(int argc, char **argv) {
    std::string root_path("");
    if (argc > 1) {
        root_path = argv[1];
        root_path += "/";
    }
    auto* stream = hope::log::create_multy_stream( {
        hope::log::create_console_stream(),
        hope::log::create_file_stream(root_path + "log.txt")
    });

    auto* logger = new hope::log::logger(*stream);

    // while (true) {
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "First msg";
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Sec msg";
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Thrd msg";
    //}

    delete logger;
    delete stream;
}
