#include "log_helper.h"

#include "hope_logger/logger.h"

#include <cstring>
#include <string_view>

namespace hope::log {

    namespace {
        std::string_view priority_to_string(log_level level){
            if(level == log_level::info)
                return "Info";
            return "Error";
        }
    }

    log_helper::log_helper(logger &logger_instance, log_level level)
        : m_logger(logger_instance) {
        char time_buf[100];
        time_t rawTime = 0;
        time(&rawTime);
        auto* time = localtime(&rawTime);
        strftime(time_buf, 100, "%d-%m-%Y %H:%M:%S", time);

        m_buffer = m_logger.alloc_buffer();
        assert(m_buffer != nullptr);
        m_buffer->put("[", 1);
        m_buffer->put(time_buf, strlen(time_buf));
        m_buffer->put("] ", 2);
        m_buffer->put("(", 1);
        auto&& priority_string = priority_to_string(level);
        m_buffer->put(priority_string.data(), priority_string.size());
        m_buffer->put(") ", 2);
    }

    log_helper::~log_helper() {
        m_logger.write_buffer(m_buffer);
    }

    void log_helper::write_impl(const void *data, std::size_t size) {
        m_buffer->put(data, size);
    }

    void log_helper::write_impl(std::string_view data) {
        write_impl(data.data(), data.size());
    }

}