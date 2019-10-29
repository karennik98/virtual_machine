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


    enum class data_token_type : uint64_t
    {
        type,
        identifire,
        assign,
        one_quote,
        open_square,
        close_square,
        number,
        two_quote,
        char_data,
        string_data,
        comma,
        hex_number
    };
    
    struct data_token
    {
        data_token_type type;
        std::string_view str;

        bool operator ==(const data_token& rhs) const
        {
            return (type == rhs.type && str == rhs.str);
        }

        bool operator !=(const data_token& rhs) const
        {
            return !(rhs == *this);
        }
    };
}