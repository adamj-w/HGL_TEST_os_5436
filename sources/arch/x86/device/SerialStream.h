#pragma once

#include "../x86.h"
#include <libruntime/Types.h>
#include <libruntime/Error.h>
#include <libsystem/Stream.h>

namespace hegel::arch::x86 {

enum class SerialPort : uint16_t
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
};

class SerialStream : public Stream
{
private:
    SerialPort _port;

    void wait_read()
    {
        while((in8(static_cast<uint16_t>(_port) + 5) & 1) == 0) {}
    }

    void wait_write()
    {
        while((in8(static_cast<uint16_t>(_port) + 5) & 0x20) == 0) {}
    }

public:
    SerialStream(SerialPort port) : _port(port)
    {
        out8(static_cast<uint16_t>(_port) + 1, 0x00);
        out8(static_cast<uint16_t>(_port) + 3, 0x80);
        out8(static_cast<uint16_t>(_port) + 0, 0x03);
        out8(static_cast<uint16_t>(_port) + 1, 0x00);
        out8(static_cast<uint16_t>(_port) + 3, 0x03);
        out8(static_cast<uint16_t>(_port) + 2, 0xC7);
        out8(static_cast<uint16_t>(_port) + 4, 0x0B);
    }

    ~SerialStream() {}

    bool writable() override
    {
        return true; 
    }

    virtual Error write_byte(byte byte) override
    {
        wait_write();

        out8(static_cast<uint16_t>(_port), byte);

        return Error::SUCCEED;
    }
};

}
