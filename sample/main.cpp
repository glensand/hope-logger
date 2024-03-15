#include "hope_logger/logger.h"
#include "hope_logger/log_helper.h"
#include "hope_logger/ostream.h"

#include <iostream>

class sstream final : public hope::log::ostream {
    virtual bool is_open() const noexcept override {
        return true;
    }

    virtual void write(const void* data, std::size_t size) override {
        std::cout.write((char*)data, size);
    }

    virtual void flush() override {

    }

public:
    sstream() = default;
};

int main() {
    auto* stream = new sstream;//hope::log::create_stream("log.log");
    auto* logger = new hope::log::logger(*stream);

    // while (true) {
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Frist msg";
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Sec msg";
        HOPE_INTERIOR_LOG(hope::log::log_level::error, *logger) << "Thrd msg";
    //}

    delete logger;
    delete stream;
}
