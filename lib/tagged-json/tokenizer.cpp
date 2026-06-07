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

}

namespace tagged_json
{

tokenizer::tokenizer(std::string const & value)
: value_(value)
, pos(0)
, line(0)
, line_pos(0)
{

}

bool tokenizer::next(token & t) noexcept
{
    try
    {
        eat_whitespace();
        switch (next_char())
        {
            case '\0':
                t.type = token_type::end;
                t.value = "";
                return false;
            case '#':
                return read_comment(t);
            case '{':
                t.type = token_type::map_begin;
                t.value = "";
                return true;
            default:
                t.type = token_type::error;
                t.value = "invalid token";
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


char tokenizer::next_char()
{
    char const result = value_[pos];
    switch (result)
    {
        case '\0':
            // end of input - do not advance
            break;
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

char tokenizer::peek() const
{
    return value_[pos];
}

}