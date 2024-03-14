/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-logger
 */

#pragma once

#include <charconv>
#include <array>
#include <string_view>

#include "hope_logger/buffer.h"
#include "hope_logger/log_level.h"

namespace hope::log {

    class logger;
    struct log_helper;

    /**
     * Helper class for log writing, provide dispatch of the writing value according their type
     */
    struct log_helper final {

        log_helper(logger& logger_instance, log_level level);
        ~log_helper();

	    template <typename T>
	    struct value_wrapper final {
            explicit value_wrapper(const T& val)
			    : value(val) { }
		
		    const T& value;
	    };

        template<typename T>
        friend log_helper& operator<<(log_helper& helper, const T& value) {
            log_write(helper, value);
            return helper;
        }

        template<typename T>
        friend void log_write(log_helper& helper, const T& val) {
            helper.write_impl(val);
        }

        /**
         *  Function needed to delegate call to the same operator<<, but with a little different semantic
         *  On MSVC it does not need, but another compilers can not call function with non const
         *  reference parameter with rvalue
         */
        template<typename T>
        friend log_helper& operator<<(log_helper&& helper, const T& value) {
            helper << value;
            return helper;
        }

	    template <typename T>
	    static value_wrapper<T> build_value(const T& value) {
		    return value_wrapper<T>(value);
	    }

        template<typename T>
        void write_impl(const value_wrapper<T>& object) {
            assert(m_buffer);
            m_buffer->put(" [", 2);
            write(object);
            m_buffer->put("] ", 2);
        }

        template<typename T>
        std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, void>
        write_impl(const T& val) {
            std::array<char, 100> buffer{};
            auto&& [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), val);
            write_impl(buffer.data(), std::size_t(ptr) - std::size_t(buffer.data()));
        }

    private:
        void write_impl(const void* data, std::size_t size);
        void write_impl(std::string_view data);

	    class buffer* m_buffer{ nullptr };	// Contains all written bytes
	    logger&	m_logger;	// Ref to the global logger, all bytes had to be written there
    };

}

#define HOPE_INTERIOR_LOG(PRIORITY, logger) \
    if ((logger).get_log_level() <= PRIORITY) \
        hope::log::log_helper((logger), PRIORITY)


#define HOPE_INTERIOR_LOG_TRACE(PRIORITY, logger) \
    if ((logger).get_log_level() <= PRIORITY) \
        hope::log::log_helper((logger), PRIORITY) <<  __FUNCTION__ << " "

#define HOPE_VAL(V) hope::log_helper::build_value(V)