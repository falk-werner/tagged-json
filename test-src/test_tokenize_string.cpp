#include <tagged-json/tagged-json.hpp>
#include <gtest/gtest.h>

TEST(tokenizer, str)
{
    tagged_json::tokenizer tokenizer("\"Hello\"");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::str, token.type);
    ASSERT_EQ("Hello", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, str_escape_quote)
{
    tagged_json::tokenizer tokenizer("\"Hello, \\\"Bob\\\"!\"");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_EQ(tagged_json::token_type::str, token.type);
    ASSERT_EQ("Hello, \"Bob\"!", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, str_escape_slash)
{
    tagged_json::tokenizer tokenizer("\"foo\\\\bar\"");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_EQ(tagged_json::token_type::str, token.type);
    ASSERT_EQ("foo\\bar", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, str_multiline)
{
    tagged_json::tokenizer tokenizer("\"1\n2\n3\"");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_EQ(tagged_json::token_type::str, token.type);
    ASSERT_EQ("1\n2\n3", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}


TEST(tokenizer, str_unterminated)
{
    tagged_json::tokenizer tokenizer("\"Hello");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::error, token.type);
}

