#include <tagged-json/tokenizer.hpp>

namespace
{

bool is_whitespace(char c)
{
    return ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\n') || (c == '\f'));
}

bool is_comment(char c)
{
    return ((c != '\n') && (c != '\r') && (c != '\0'));
}

bool is_digit(char c)
{
    return (('0' <= c) && (c <= '9'));
}

}

namespace tagged_json
{

tokenizer::tokenizer(std::string const & value)
: value_(value)
, done(false)
, pos(0)
, line(0)
, line_pos(0)
{

}

bool tokenizer::next(token & t) noexcept
{
    try
    {
        t.value = "";
        t.u_value = 0;

        eat_whitespace();

        if (is_digit(peek())) {
            return read_uint(t);
        }

        switch (next_char())
        {
            case '\0':
                t.type = token_type::end;
                done = true;
                return false;
            case '#':
                return read_comment(t);
            case 'n':
                return read_keyword(t, "none", token_type::none);
            case 't':
                return read_keyword(t, "true", token_type::bool_true);
            case 'f':
                return read_keyword(t, "false", token_type::bool_false);
            case '[':
                t.type = token_type::seq_begin;
                return true;
            case ']':
                t.type = token_type::seq_end;
                return true;
            case ',':
                t.type = token_type::item_separator;
                return true;
            case '{':
                t.type = token_type::map_begin;
                return true;
            case '}':
                t.type = token_type::map_end;
                return true;
            case '(':
                t.type = token_type::ordered_map_begin;
                return true;
            case ')':
                t.type = token_type::ordered_map_end;
                return true;
            case ':':
                t.type = token_type::key_value_separator;
                return true;
            case ';':
                t.type = token_type::trailer;
                done = true;
                return false;
            default:
                t.type = token_type::error;
                t.value = "invalid token";
                done = true;
                return false;
        }
    }
    catch (std::exception const & ex)
    {
        t.type = token_type::error;
        t.value = ex.what();
        return false;
    }
    catch (...)
    {
        t.type = token_type::error;
        t.value = "error";
        return false;
    }

}

void tokenizer::eat_whitespace()
{
    while (is_whitespace(peek())) {
        next_char();
    }
}

bool tokenizer::read_comment(token & t)
{
    t.type = token_type::comment;
    t.value = "";
    while (is_comment(peek())) {
        t.value += next_char();
    }

    return true;
}

bool tokenizer::read_keyword(token & t, std::string const & keyword, token_type type)
{
    for(size_t i = 1; i < keyword.size(); i++) {
        if (keyword[i] != next_char()) {
            t.type = token_type::error;
            t.value = keyword + " expected";
            return false;
        }
    }

    t.type = type;
    t.value = "";
    return true;
}

bool tokenizer::read_uint(token & t)
{
    uint64_t value = 0;
    while (is_digit(peek())) {
        auto const c = next_char();
        value *= 10;
        value += c - '0';
    }

    t.type = token_type::uint;
    t.u_value = value;
    return true;
}

char tokenizer::next_char()
{
    if ((!done) && (pos < value_.size()))
    {
        char const result = value_[pos];
        switch (result)
        {
            case '\n':
                pos++;
                line++;
                line_pos = 0;
                break;
            default:
                pos++;
                line_pos++;
                break;
        }
        return result;
    }

    done = true;
    return '\0';
}

char tokenizer::peek() const
{
    return (!done) ? value_[pos] : '\0';
}

}