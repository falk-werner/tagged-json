#include <tagged-json/tagged-json.hpp>
#include <climits>
#include <gtest/gtest.h>

TEST(tokenizer, f64)
{
    tagged_json::tokenizer tokenizer("10.0");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::f64, token.type);
    ASSERT_EQ(10.0, token.f64_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}

TEST(tokenizer, f64_negative_value)
{
    tagged_json::tokenizer tokenizer("-2.0");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::f64, token.type);
    ASSERT_EQ(-2.0, token.f64_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}

TEST(tokenizer, f64_exponent)
{
    tagged_json::tokenizer tokenizer("10e2");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result) << token.value;
    ASSERT_EQ(tagged_json::token_type::f64, token.type);
    ASSERT_EQ(1000.0, token.f64_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}

TEST(tokenizer, f64_fraction_and_exponent)
{
    tagged_json::tokenizer tokenizer("-10.5e2");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result) << token.value;
    ASSERT_EQ(tagged_json::token_type::f64, token.type);
    ASSERT_EQ(-1050.0, token.f64_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}

TEST(tokenizer, f64_negative_exponent)
{
    tagged_json::tokenizer tokenizer("100e-1");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result) << token.value;
    ASSERT_EQ(tagged_json::token_type::f64, token.type);
    ASSERT_EQ(10.0, token.f64_value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}