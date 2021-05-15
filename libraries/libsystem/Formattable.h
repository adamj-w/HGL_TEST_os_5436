#pragma once

#include <libsystem/Format.h>
#include <libruntime/Macros.h>

namespace hegel {

class Formattable
{
public:
    virtual ErrorOr<size_t> format(Stream& stream, FormatInfo& info)
    {
        __unused(info);

        return hegel::format(stream, "Object({#x})", reinterpret_cast<void*>(this));
    }
};

}