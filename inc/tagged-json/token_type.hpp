#ifndef TAGGED_JSON_TOKEN_TYPE_HPP
#define TAGGED_JSON_TOKEN_TYPE_HPP

namespace tagged_json
{

enum class token_type
{
    comment,
    none,
    bool_true,
    bool_false,
    uint,
    sint,
    f64,
    str,
    bin,
    seq_begin,
    seq_end,
    ordered_map_begin,
    ordered_map_end,
    map_begin,
    map_end,
    tag,
    error,
    trailer,
    end
};

}

#endif
