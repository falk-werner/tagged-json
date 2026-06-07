#include <tagged-json/tagged-json.hpp>
#include <gtest/gtest.h>

TEST(tokenizer, keyword_none)
{
    tagged_json::tokenizer tokenizer("none");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::none, token.type);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, keyword_true)
{
    tagged_json::tokenizer tokenizer("true");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::bool_true, token.type);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, keyword_false)
{
    tagged_json::tokenizer tokenizer("false");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::bool_false, token.type);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}
