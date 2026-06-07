#ifndef TAGGED_JSON_TOKEN_TYPE_HPP
#define TAGGED_JSON_TOKEN_TYPE_HPP

namespace tagged_json
{

enum class token_type
{
    comment,                // #*$
    none,                   // keyword: none
    bool_true,              // keyword: true
    bool_false,             // keyword: false
    uint,                   // [0-9]+
    sint,                   // [+\-][0-9]+
    f64,                    // [+\-][0-9]+\.[0-9]+
    str,                    // "[^"]*"
    bin,                    // b'[0-9a-zA-Z\+/]*'
    seq_begin,              // [
    seq_end,                // ]
    item_separator,         // ,
    ordered_map_begin,      // (
    ordered_map_end,        // )
    map_begin,              // {
    map_end,                // }
    key_value_separator,    // :
    tag,                    // @\w+
    trailer,                // ;
    error,
    end
};

}

#endif
