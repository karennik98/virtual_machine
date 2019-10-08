#pragma once

#include <string_view>
#include <cstdint>

namespace vm
{
    enum class token_type : uint64_t
    {
        undef,
        comma,
        semicolon,
        open_square,
        close_square,
        identifire,
        number
    };

    struct token
    {
        token_type type;
        std::string_view str;

        bool operator ==(const token& rhs) const
        {
            return (type == rhs.type && str == rhs.str);
        }

        bool operator !=(const token& rhs) const
        {
            return !(rhs == *this);
        }
    };
}