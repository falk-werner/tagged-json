#include <tagged-json/tagged-json.hpp>
#include <gtest/gtest.h>

TEST(tokenizer, empty)
{
    tagged_json::tokenizer tokenizer("");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, whitespace_only)
{
    tagged_json::tokenizer tokenizer(" \t\r\n\f");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, complex)
{
    std::string const text = R"(
    # This is a comment.
    {
        "version": 2,
        "list": [none, true, false, 42],
        "ordered": ( "foo": "bar", "baz": none )
    };
    garbage at the end is ignored
    )";
    tagged_json::tokenizer tokenizer(text);

    std::vector<tagged_json::token_type> tokens = {
        tagged_json::token_type::comment,
        tagged_json::token_type::map_begin,

        tagged_json::token_type::str,
        tagged_json::token_type::key_value_separator,
        tagged_json::token_type::uint,
        tagged_json::token_type::item_separator,

        tagged_json::token_type::str,
        tagged_json::token_type::key_value_separator,
        tagged_json::token_type::seq_begin,
        tagged_json::token_type::none,
        tagged_json::token_type::item_separator,
        tagged_json::token_type::bool_true,
        tagged_json::token_type::item_separator,
        tagged_json::token_type::bool_false,
        tagged_json::token_type::item_separator,
        tagged_json::token_type::uint,
        tagged_json::token_type::seq_end,
        tagged_json::token_type::item_separator,

        tagged_json::token_type::str,
        tagged_json::token_type::key_value_separator,
        tagged_json::token_type::ordered_map_begin,
        tagged_json::token_type::str,
        tagged_json::token_type::key_value_separator,
        tagged_json::token_type::str,
        tagged_json::token_type::item_separator,
        tagged_json::token_type::str,
        tagged_json::token_type::key_value_separator,
        tagged_json::token_type::none,
        tagged_json::token_type::ordered_map_end,

        tagged_json::token_type::map_end
    };

    tagged_json::token token;
    bool result;

    for(auto const type: tokens) {
        result = tokenizer.next(token);
        ASSERT_TRUE(result);
        ASSERT_EQ(type, token.type);
    }

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::trailer, token.type);
    
    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}
