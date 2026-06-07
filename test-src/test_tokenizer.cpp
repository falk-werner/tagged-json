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

TEST(tokenizer, comment)
{
    tagged_json::tokenizer tokenizer("# foo");

    tagged_json::token token;
    bool result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ(" foo", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, multiple_comments)
{
    tagged_json::tokenizer tokenizer("#foo\n#bar\n#baz");

    tagged_json::token token;
    bool result;

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("foo", token.value);

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("bar", token.value);

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("baz", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}

TEST(tokenizer, multiple_comments_win_lineending)
{
    tagged_json::tokenizer tokenizer("#foo\r\n#bar\r\n#baz");

    tagged_json::token token;
    bool result;

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("foo", token.value);

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("bar", token.value);

    result = tokenizer.next(token);
    ASSERT_TRUE(result);
    ASSERT_EQ(tagged_json::token_type::comment, token.type);
    ASSERT_EQ("baz", token.value);

    result = tokenizer.next(token);
    ASSERT_FALSE(result);
    ASSERT_EQ(tagged_json::token_type::end, token.type);
}
