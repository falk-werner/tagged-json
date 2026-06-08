#ifndef TAGGED_JSON_TOKENIZER_HPP
#define TAGGED_JSON_TOKENIZER_HPP

#include <tagged-json/token.hpp>
#include <string>

namespace tagged_json
{

class tokenizer
{
public:
    explicit tokenizer(std::string const & value);
    ~tokenizer() = default;

    bool next(token& t) noexcept;
private:
    void eat_whitespace();
    bool read_comment(token & t);
    bool read_keyword(token & t, std::string const & keyword, token_type type);
    bool read_uint(token & t);
    bool read_sint(token & t, bool is_negative);
    bool read_f64(token & t);
    bool read_str(token & t);
    bool read_tag(token & t);
    char next_char();
    char peek() const;

    std::string const value_;
    bool done;
    size_t pos;
    size_t line;
    size_t line_pos;
};

}

#endif
