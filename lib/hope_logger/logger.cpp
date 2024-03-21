#include "logger.h"
#include "hope_logger/ostream.h"
#include "hope_logger/buffer.h"

namespace hope::log {

    logger::~logger() noexcept {
        if(!m_enabled.load(std::memory_order_acquire))
            return;
        try {
            m_enabled.store(false, std::memory_order_release);
            m_message_added.notify_one();
            m_writing_thread.join();
            write_task();
            m_stream.write("\0", 1);
        } catch(...) {
            // Ignore
        }
    }

    logger::logger(ostream &stream, std::size_t system_max_threads_count)
        : m_stream(stream)
        , m_buffer_pool(system_max_threads_count) {
        m_enabled.store(true, std::memory_order_release);
        m_writing_thread = std::thread([this]{ 
            while(m_enabled.load(std::memory_order_acquire)) {
                write_task();
                std::unique_lock lk(m_message_added_mutex);
                m_message_added.wait(lk);
            }
        });
    }

    void logger::write_buffer(buffer* message) {
        m_write_queue.enqueue(message);
        m_message_added.notify_one();
    }

    buffer *logger::alloc_buffer() {
        buffer* buffer_instance{ nullptr };
        if (!m_buffer_pool.try_dequeue(buffer_instance)){
            buffer_instance = new buffer;
        }
        return buffer_instance;
    }

    void logger::write_task() {
        buffer* wb{ nullptr };
        while (m_write_queue.try_dequeue(wb)) {
            m_stream.write(wb->m_buffer_impl.data(), wb->m_bytes_written);
            m_stream.write(wb->m_additional_buffer.data(), wb->m_additional_buffer.size());
            if (!m_buffer_pool.try_enqueue(wb)){
                delete wb;
            }
        }
        m_stream.flush();
    }

}