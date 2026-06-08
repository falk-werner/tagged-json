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
        t.i_value = 0;
        t.f64_value = 0.0;

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
            case '+':
                return read_sint(t, false);
            case '-':
                return read_sint(t, true);
            case '\"':
                return read_str(t);
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
            case '@':
                return read_tag(t);
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
    t.value = "";
    t.u_value = 0;
    while (is_digit(peek())) {
        uint64_t const old_value = t.u_value;

        auto const c = next_char();
        t.value += c;
        t.u_value *= 10;
        t.u_value += c - '0';

        if (old_value > t.u_value) {
            t.type = token_type::error;
            t.value = "uint overflow";
            t.u_value = 0;
            done = true;
            return false;
        }
    }

    auto const next = peek();
    if ((next == '.') || (next == 'e') || (next == 'E')) {
        return read_f64(t);
    }

    t.type = token_type::uint;
    return true;
}

bool tokenizer::read_sint(token & t,  bool is_negative)
{
    if (read_uint(t)) {
        if (t.type == token_type::f64) {
            if (is_negative) {
                t.f64_value = -t.f64_value;
            }
            return true;
        }

        if (!is_negative) {
            if (t.u_value <= INT64_MAX) {
                t.type = token_type::sint;
                t.i_value = static_cast<int64_t>(t.u_value);
                return true;
            }
            else {
                t.type = token_type::error;
                t.value = "integer overflow";
                return false;
            }
        }
        else {
            if (t.u_value <= (static_cast<uint64_t>(INT64_MAX) + 1)) {
                t.type = token_type::sint;
                t.u_value = ~t.u_value + 1;
                void * p = reinterpret_cast<void*>(&(t.u_value));
                t.i_value = *(reinterpret_cast<int64_t*>(p));
                return true;
            }
            else {
                t.type = token_type::error;
                t.value = "integer underflow";
                return false;
            }
        }

    }
    else {
        return false;
    }
}

bool tokenizer::read_f64(token & t)
{
    auto next = peek();
    if (next == '.') {
        t.value += next_char();
        while (is_digit(peek())) {
            t.value += next_char();
        }
    }

    next = peek();
    if ((next == 'e') || (next == 'E')) {
        t.value += next_char();

        next = peek();
        if ((next == '-') || (next == '+')) {
            t.value += next_char();
        }

        while (is_digit(peek())) {
            t.value += next_char();
        }
    }

    try {
        t.f64_value = std::stod(t.value);
        t.type = token_type::f64;
        return true;
    }
    catch (std::exception const & ex) {
        t.value = ex.what();
        t.type = token_type::error;
        return false;
    }
    catch (...) {
        t.value = "invalid f64 value";
        t.type = token_type::error;
        return false;
    }
}


bool tokenizer::read_str(token & t)
{
    std::string value;

    for(;;) {
        char const c = next_char();
        switch (c)
        {
            case '\"':
                t.type = token_type::str;
                t.value = value;
                return true;
            case '\\':
                {
                    char const n = next_char();
                    switch (n)
                    {
                        case '\\':
                            value += '\\';
                            break;
                        case '\"':
                            value += '\"';
                            break;
                        default:
                            t.type = token_type::error;
                            t.value = "invalid escape sequence";
                            done = false;
                            return false;
                    }
                }
                break;
            case '\0':
                t.type = token_type::error;
                t.value = "unexpected end of string";
                            done = false;
                return false;
            default:
                value += c;
                break;
        }
    }
}

bool tokenizer::read_tag(token & t)
{
    while ((!is_whitespace(peek())) && ('\0' != peek())) {
        t.value += next_char();
    }

    t.type = token_type::tag;
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