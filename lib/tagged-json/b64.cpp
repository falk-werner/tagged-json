#include "tagged-json/b64.hpp"

namespace
{

unsigned char b64_value(char c)
{
    if (('A' <= c) && (c <= 'Z')) {
        return static_cast<unsigned char>(c - 'A');
    }
    else if (('a' <= c) && (c <= 'z')) {
        return static_cast<unsigned char>(c - 'a' + 26);
    }
    else if (('0' <= c) && (c <= '9')) {
        return static_cast<unsigned char>(c - '0' + 52);
    }
    else if ('+' == c) {
        return 62;
    }
    else if ('/' == c) {
        return 63;
    }
    else {
        return 0x80;
    }
}

}

namespace tagged_json
{

bool b64_decode(std::string & value)
{
    if (0 == value.size()) { return true; }
    if (value.size() % 4 != 0) { return false; }

    size_t in = 0;
    size_t out = 0;
    if (value.size() > 4) {
        while (in < value.size() - 4) {
            auto const a = b64_value(value[in++]);
            auto const b = b64_value(value[in++]);
            auto const c = b64_value(value[in++]);
            auto const d = b64_value(value[in++]);

            if ((a == 0x80) || (b == 0x80) || (c == 0x80) || (d == 0x80)) {
                return false;
            }

            value[out++] = (char) ( (a << 2) | (b >> 4));
            value[out++] = (char) ( (b << 4) | (c >> 2));
            value[out++] = (char) ( (c << 6) | d);
        }
    }

    if (in < value.size()) {
        if ((value[in + 2] == '=') && (value[in + 3] != '=')) {
            return false;
        }

        auto const a = b64_value(value[in++]);
        auto const b = b64_value(value[in++]);

        if ((a == 0x80) || (b == 0x80)) {
            return false;
        }

        value[out++] = (char) ( (a << 2) | (b >> 4));
        
        if (value[in] != '=') {
            auto const c = b64_value(value[in++]);
            if (c == 0x80) { return false; }
            value[out++] = (char) ( (b << 4) | (c >> 2));

            if (value[in] != '=') {
                auto const d = b64_value(value[in++]);
                if (d == 0x80) { return false; }
                value[out++] = (char) ( (c << 6) | d);
            }
        }
    }

    value.resize(out);
    return true;
}

}