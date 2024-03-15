#include "ostream.h"
#include <fstream>

namespace hope::log {

    class ofstream final : public ostream {
    public:

        explicit ofstream(std::string_view file_name){
            // assumed binary append mode is best choice for default stream
            m_stream_impl.open(file_name.data(), std::ios::binary | std::ios::app);
        }

        bool is_open() const noexcept override {
            return m_stream_impl.is_open();
        }

        void write(const void* data, std::size_t size) override {
            m_stream_impl.write((const char*)data, (long)size);
        }

        void flush() override {
            m_stream_impl.flush();
        }

    private:
        std::ofstream m_stream_impl;
    };

    ostream* create_stream(std::string_view file_name) {
        return new ofstream(file_name);
    }

}