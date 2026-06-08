#ifndef TAGGED_JSON_TOKEN_HPP
#define TAGGED_JSON_TOKEN_HPP

#include <tagged-json/token_type.hpp>
#include <cinttypes>
#include <string>

namespace tagged_json
{

struct token
{
    token_type type;
    std::string value;
    uint64_t u_value;
    int64_t i_value;
    double f64_value;
};

}

#endif
