#include "buffer.h"

namespace hope::log {

    void buffer::put(const void *data, std::size_t size) noexcept {
        auto&& free_space = BufferSize - m_bytes_written;
        auto write_to_stack_buffer = free_space > size ? size : free_space;
        auto* begin = (char*)data;
        std::copy(begin, begin + write_to_stack_buffer, m_buffer_impl.data() + m_bytes_written);
        m_bytes_written += write_to_stack_buffer;
        // if some amount of bytes does not fit to the stack buffer, copy it to the heap
        for(std::size_t i{ write_to_stack_buffer }; i < size; ++i){
            m_additional_buffer.emplace_back(begin[i]);
        }
    }

    void buffer::clear() {
        m_additional_buffer.resize(0);
        m_bytes_written = 0;
    }
}
