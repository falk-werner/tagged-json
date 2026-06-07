#include <tagged-json/tagged-json.hpp>
#include <climits>
#include <gtest/gtest.h>

TEST(tokenizer, uint_value)
{
    tagged_json::tokenizer tokenizer("42");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::uint, token.type);
    ASSERT_EQ(42, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, uint_max)
{
    tagged_json::tokenizer tokenizer(std::to_string(UINT64_MAX));

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::uint, token.type);
    ASSERT_EQ(UINT64_MAX, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, uint_overflow)
{
    tagged_json::tokenizer tokenizer(std::to_string(UINT64_MAX) + "0");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::error, token.type);
}
