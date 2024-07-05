/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-logger
 */

#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "hope_logger/log_level.h"
#include "hope_logger/buffer.h"

#include "misc/logger_mpmc_bounded_queue.h"
#include "misc/logger_mpsc_queue.h"

namespace hope::log {

    class ostream;
    class buffer;

    class logger final {
    public:

	    explicit logger(ostream& stream, std::size_t system_max_threads_count 
            = std::thread::hardware_concurrency(), bool use_flush_thread = true);
	    ~logger() noexcept;

        [[maybe_unused]] log_level get_log_level() const noexcept { return m_log_level; }
        void set_log_level(log_level level) noexcept { m_log_level = level; }

    private:

        buffer* alloc_buffer();
        void write_buffer(buffer* message);
        void write_task();
        

        mpmc_bounded_queue<buffer*> m_buffer_pool;
        logger_mpsc_queue<buffer*> m_write_queue; // todo:: use buffered queue

        std::thread m_writing_thread;
        ostream& m_stream;
        std::atomic_bool m_enabled{ false };
        log_level m_log_level{ log_level::info };

        std::condition_variable m_message_added;
        std::mutex m_message_added_mutex;

        bool m_use_flush_thread;
	    friend struct log_helper;
    };

}
