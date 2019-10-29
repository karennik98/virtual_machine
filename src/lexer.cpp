#include "../include/lexer.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
#include <optional>
#include <string_view>
#include <cassert>

namespace vm
{
    lexer::lexer(const std::string& source)
        : m_char{ source.cbegin() }
        , m_end{ source.cend()}
    {}

    std::vector<token> lexer::lex()
    {
        std::vector<token> tokens;

        while(!is_at_end())
        {
            eat_whitespaces();
            if(is_at_end())
            {
                break;
            }

            const auto tok = get_next_token();
            tokens.emplace_back(tok);
        }

        return tokens;
    }

    std::vector<data_token> lex_data()
    {
        std::vector<data_token> tokens;

        while(!is_at_end_data_segment())
        {
            
        }
    }

    token lexer::get_next_token()
    {
        if(is_current_one_char_token())
        {
            return get_one_char_token();
        }
        if(is_current_identifier_token())
        {
            return get_identifier_token();
        }
        if(is_current_number_token())
        {
            return get_number_token();
        }
        return {};
    }

    token lexer::get_one_char_token()
    {
        const auto type = [ch = current()]
        {
            switch (ch)
            {
                case ',': return token_type::comma;
                case '[': return token_type::open_square;
                case ']': return token_type::close_square;
                case ':': return token_type::semicolon;

                default:
                    assert(false && "Unexpected one char token");
            }
        }();

        const auto str = std::string_view{&(current_cref()), 1u};
        eat();

        return {type, str};
    }

    token lexer::get_identifier_token()
    {
        const auto pred = [this](const char ch)
        {
            return !is_identifier_char(ch) && !std::isdigit(ch);
        };

        return get_and_eat_token_for_predicate(token_type::identifire, pred);
    }   

    token lexer::get_number_token()
    {
        const auto pred = [this](const char ch)
        {
            return !std::isdigit(ch);
        };

        return get_and_eat_token_for_predicate(token_type::number, pred);
    }

    template <typename Pred>
    token lexer::get_and_eat_token_for_predicate(token_type type, Pred&& pred)
    {
        const auto begin = m_char;
        const auto found = std::find_if(begin, m_end, pred);
        const auto str = std::string_view{ &current_cref(),
                                            static_cast<std::string_view::size_type>(std::distance(begin, found))};
        
        eat(str.size());
        return {type, str};
    }

    void lexer::eat(unsigned number)
    {
        std::advance(m_char, number);
    }

    void lexer::eat_whitespaces()
    {
        const auto begin = m_char;
        const auto pred = [](const char ch)
        {
            return !std::isspace(ch);
        };
        const auto found = std::find_if(begin, m_end, pred);
        const auto whitespaces_count = std::distance(begin, found);
        if(whitespaces_count > 0)
        {
            eat(whitespaces_count);
        }
    }

    char lexer::current() const
    {
        return *m_char;
    }
    const char& lexer::current_cref() const
    {
        return *m_char;
    }

    bool lexer::is_at_end() const
    {
        return m_char == m_end;
    }

    bool lexer::is_at_end_data_segment() const
    {
        return *m_char == ".CODE";
    }

    bool lexer::is_identifier_char(char ch) const
    {
        return std::isalpha(ch) || ch == '_';
    }

    bool lexer::is_current_one_char_token() const
    {
        const auto chars = {',','[',']',':'};
        return !is_at_end() 
            && std::any_of(std::cbegin(chars), std::cend(chars),
                [current = current()](const char c){ return current == c; });
    }

    bool lexer::is_current_identifier_token() const
    {
        return !is_at_end() && is_identifier_char(current());
    }

    bool lexer::is_current_number_token() const
    {
        return !is_at_end() && std::isdigit(current());
    }
}
   