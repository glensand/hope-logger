#include "ostream.h"

#include <fstream>
#include <algorithm>
#include <iostream>

namespace hope::log {

    ostream* create_file_stream(std::string_view file_name) {
        class ofstream final : public ostream {
        public:

            explicit ofstream(std::string_view file_name){
                // assumed binary append mode is best choice for default stream
                m_stream_impl.open(file_name.data(), std::ios::binary | std::ios::app);
            }

            virtual bool is_open() const noexcept override {
                return m_stream_impl.is_open();
            }

            virtual void write(const void* data, std::size_t size) override {
                m_stream_impl.write((const char*)data, (long)size);
            }

            virtual void flush() override {
                m_stream_impl.flush();
            }

        private:
            std::ofstream m_stream_impl;
        };

        return new ofstream(file_name);
    }

    ostream *create_multy_stream(std::vector<ostream*> &&in_streams) {
        class multy_stream final : public ostream {
        public:
            explicit multy_stream(std::vector<ostream*>&& streams)
                : m_streams(std::move(streams)){}

            virtual ~multy_stream() override {
                for (auto* stream : m_streams)
                    delete stream;
            }

            virtual bool is_open() const noexcept override {
                return std::ranges::all_of(m_streams, 
                    [](auto* stream){ return stream->is_open();});
            }

            virtual void write(const void* data, std::size_t size) override {
                for (auto* stream : m_streams)
                    stream->write(data, size);
            }

            virtual void flush() override {
                for (auto* stream : m_streams)
                    stream->flush();
            }
        private:
            std::vector<ostream*> m_streams;
        };

        return new multy_stream(std::move(in_streams));
    }

    ostream *create_console_stream() {
        class console_stream final : public ostream {
        public:
            console_stream() = default;

            virtual bool is_open() const noexcept override {
                return true;
            }

            virtual void write(const void* data, std::size_t size) override {
                std::cout.write((const char*)data, size);
            }

            virtual void flush() override { }
        };

        return new console_stream;
    }

}