#include <tagged-json/tagged-json.hpp>
#include <climits>
#include <gtest/gtest.h>

TEST(tokenizer, sint_postive_value)
{
    tagged_json::tokenizer tokenizer("+42");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::sint, token.type);
    ASSERT_EQ(42, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, sint_max)
{
    tagged_json::tokenizer tokenizer(std::string("+") + std::to_string(INT64_MAX));

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::sint, token.type);
    ASSERT_EQ(INT64_MAX, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, sint_overflow)
{
    tagged_json::tokenizer tokenizer(std::string("+") + std::to_string( ((uint64_t)INT64_MAX) + 1));

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::error, token.type);
}

TEST(tokenizer, sint_negative_value)
{
    tagged_json::tokenizer tokenizer("-42");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::sint, token.type);
    ASSERT_EQ(-42, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, sint_min)
{
    tagged_json::tokenizer tokenizer(std::to_string(INT64_MIN));

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::sint, token.type);
    ASSERT_EQ(INT64_MIN, token.u_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, sint_underflow)
{
    tagged_json::tokenizer tokenizer(std::string("-") + std::to_string( ((uint64_t)INT64_MAX) + 2));

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::error, token.type);
}
