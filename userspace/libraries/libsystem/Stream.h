#pragma once

#include <libruntime/ErrorOr.h>
#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>

namespace hegel {

enum SeekOrigin
{
    BEGIN,
    CURRENT,
    END,
};

class Stream : public RefCounted<Stream>
{
public:
    typedef int64_t Offset;

    Stream();
    ~Stream();

    virtual bool readable();
    virtual bool writable();
    virtual bool seekable();

    virtual ErrorOr<size_t> read(void* buffer, size_t size);
    virtual ErrorOr<size_t> write(const void* buffer, size_t size);
    virtual ErrorOr<byte> read_byte();
    virtual Error write_byte(byte byte);
    virtual ErrorOr<size_t> seek(Stream::Offset offset, SeekOrigin origin);
    virtual ErrorOr<size_t> tell();

    virtual void flush();
};

}