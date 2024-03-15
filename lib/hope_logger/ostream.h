/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <string_view>

namespace hope::log {

    class ostream {
    public:
        virtual ~ostream() = default;
        
        [[nodiscard]] virtual bool is_open() const noexcept = 0;
        virtual void write(const void* data, std::size_t size) = 0;
        virtual void flush()  = 0;
    };

    ostream* crate_stream(std::string_view file_name);

}
