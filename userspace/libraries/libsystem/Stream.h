#pragma once

#include <libsystem/ErrorOr.h>
#include <libutil/RefCounted.h>
#include <libsystem/Types.h>

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

    virtual ErrorOrSizeT read(void* buffer, size_t size);
    virtual ErrorOrSizeT write(const void* buffer, size_t size);
    virtual ResultOr<byte> read_byte();
    virtual Result write_byte(byte byte);
    virtual ErrorOrSizeT seek(Stream::Offset offset, SeekOrigin origin);
    virtual ErrorOrSizeT tell();

    virtual void flush();
};

}