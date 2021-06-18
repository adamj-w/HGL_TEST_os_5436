#pragma once

#include <libsystem/Types.h>
#include <libsystem/Stream.h>

namespace hegel {

class Codepoint
{
private:
    char32_t _value;

public:
    Codepoint() : _value(0) {}

    Codepoint(char32_t value) : _value(value) {}

    ~Codepoint() {}

    bool is_digit()
    {
        return (_value >= U'0' && _value <= U'9');
    }

    bool is_alpha()
    {
        return (_value >= U'a' && _value <= U'z') ||
               (_value >= U'A' && _value <= U'Z');
    }

    int numeric_value()
    {
        if (is_digit())
        {
            return _value - U'0';
        }
        else
        {
            return 0;
        }
    }

    operator char32_t() { return _value; }
    bool operator==(char32_t other) { return _value == other; }

    char to_cp437();
};

class UTF8Stream : public Stream
{
private:
    bool _is_decoding = false;
    char32_t _current_decoding = 0;
    int _width_decoding = 0;

public: 
    virtual Result write_byte(uint8_t byte)
    {
        if (!_is_decoding)
        {
            if ((byte & 0xf8) == 0xf0)
            {
                _current_decoding = (0x07 & byte) << 18;

                _is_decoding = true;
                _width_decoding = 3;
            }
            else if ((byte & 0xf0) == 0xe0)
            {
                _current_decoding = (0x0f & byte) << 12;

                _is_decoding = true;
                _width_decoding = 2;
            }
            else if ((byte & 0xe0) == 0xc0)
            {
                _current_decoding = (0x1f & byte) << 6;

                _is_decoding = true;
                _width_decoding = 1;
            }
            else
            {
                _current_decoding = byte;

                _is_decoding = true;
                _width_decoding = 0;
            }
        }
        else
        {
            if ((byte & 0b11000000) == 0b10000000)
            {
                _width_decoding--;
                _current_decoding |= (0x3f & byte) << (6 * _width_decoding);
            }
            else
            {
                _is_decoding = false;
                return write_codepoint(Codepoint(U'?'));
            }
        }

        if (_is_decoding && _width_decoding == 0)
        {
            _is_decoding = false;
            return write_codepoint(Codepoint(_current_decoding));
        }

        return Result::SUCCEED;
    }

    virtual Result write_codepoint(Codepoint ch) = 0;
};

}