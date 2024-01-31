/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <string_view>
#include <vector>
#include <array>

namespace hope::log {

    class buffer final
    {
    public:
        // Size in bytes of the inner buffer this value assumed to be never reached
	    static constexpr std::size_t BufferSize = 1023;
	    using buffer_t = std::array<char, BufferSize>;

        buffer() = default;
        ~buffer() = default;

	    void put(const void* data, std::size_t size) noexcept;
        void clear();

        buffer_t m_buffer_impl{ };
        std::vector<char> m_additional_buffer;
        std::size_t m_bytes_written{ 0 };
    };

}
