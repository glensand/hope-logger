#include "ostream.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <filesystem>

namespace hope::log {

    ostream* create_file_stream(std::string file_name, std::size_t file_max_size) {
        class ofstream final : public ostream {
        public:

            explicit ofstream(std::string file_name, std::size_t file_max_size) 
                : m_file_name(std::move(file_name))
                , m_file_max_size(file_max_size)
            {
                std::filesystem::path p(m_file_name);
                if (!p.parent_path().empty()){
                    std::filesystem::create_directories(p.parent_path());
                }

                create_stream();
            }

            virtual bool is_open() const noexcept override {
                return m_stream_impl.is_open();
            }

            virtual void write(const void* data, std::size_t size) override {
                m_stream_impl.write((const char*)data, (long)size);
                m_file_size += size;
                if (m_file_size >= m_file_max_size) {
                    m_file_size = 0;
                    m_stream_impl.close();
                    m_stream_impl = std::ofstream();
                    create_stream();
                }
            }

            virtual void flush() override {
                m_stream_impl.flush();
            }

        private:
            void create_stream() {
                // assumed binary append mode is best choice for default stream
                std::filesystem::path p(m_file_name);
                const auto file_name = p.filename().string();
                const auto path = p.parent_path().string();
                char time_buf[100];
                time_t rawTime = 0;
                time(&rawTime);
                auto* time = localtime(&rawTime);
                strftime(time_buf, 100, "%d_%m_%Y_%H_%M_%S_", time);
                std::string timed_file_name;
                if (path.empty()) {
                    timed_file_name = time_buf + m_file_name;
                } else {
                    timed_file_name = path + "/" + time_buf + file_name;
                }
                m_stream_impl.open(timed_file_name, std::ios::binary | std::ios::app);
            }

            std::string m_file_name;
            const std::size_t m_file_max_size;
            
            std::ofstream m_stream_impl;
            std::size_t m_file_size = 0;
        };

        return new ofstream(std::move(file_name), file_max_size * 1024 * 1024);
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
                for (auto* stream : m_streams)
                    if (!stream->is_open()) return false;
                return true;
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