#include <libsystem/Macros.h>
#include <libsystem/Stream.h>

namespace hegel {

Stream::Stream() {}
Stream::~Stream() {}

bool Stream::readable() 
{
    return false;
}

bool Stream::writable()
{
    return false;
}

bool Stream::seekable()
{
    return false;
}

ErrorOrSizeT Stream::read(void* buffer, size_t size)
{
    size_t readed;

    for(readed = 0; readed < size; readed++) {
        auto result = read_byte();

        if(result != Error::SUCCEED) {
            return ErrorOrSizeT(result.error(), readed);
        } else {
            ((byte*)buffer)[readed] = result.value();
        }
    }

    return ErrorOrSizeT(readed);
}

ErrorOrSizeT Stream::write(const void* buffer, size_t size)
{
    size_t written;

    for(written = 0; written < size; written++) {
        auto result = write_byte(((byte*)buffer)[written]);

        if(result != Error::SUCCEED) {
            return ErrorOrSizeT(result, written);
        }
    }

    return ErrorOrSizeT(written);
}

ErrorOr<byte> Stream::read_byte()
{
    return ErrorOr<byte>(Error::NOT_IMPLEMENTED);
}

Error Stream::write_byte(byte byte)
{
    __unused(byte);

    return Error::NOT_IMPLEMENTED;
}

ErrorOrSizeT Stream::seek(Stream::Offset offset, SeekOrigin origin)
{
    __unused(offset);
    __unused(origin);

    return ErrorOrSizeT(Error::NOT_IMPLEMENTED);
}

ErrorOrSizeT Stream::tell()
{
    return ErrorOrSizeT(Error::NOT_IMPLEMENTED);
}

void Stream::flush() {}

}