#include <tagged-json/tagged-json.hpp>
#include <gtest/gtest.h>

TEST(tokenizer, tag)
{
    tagged_json::tokenizer tokenizer("@foo");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::tag, token.type);
    ASSERT_EQ("foo", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);    
}