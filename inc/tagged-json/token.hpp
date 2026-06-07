#ifndef TAGGED_JSON_TOKEN_HPP
#define TAGGED_JSON_TOKEN_HPP

#include <tagged-json/token_type.hpp>
#include <string>

namespace tagged_json
{

struct token
{
    token_type type;
    std::string value;
};

}

#endif
