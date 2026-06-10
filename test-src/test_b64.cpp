#include <gtest/gtest.h>
#include "tagged-json/b64.hpp"

using tagged_json::b64_decode;

TEST(b64, empty)
{
    std::string value;
    ASSERT_TRUE(b64_decode(value));
    ASSERT_EQ("", value);
}

TEST(b64, decode_hello_world)
{
    std::string value("SGVsbG8sIFdvcmxkIQ==");
    ASSERT_TRUE(b64_decode(value));
    ASSERT_EQ("Hello, World!", value);

}