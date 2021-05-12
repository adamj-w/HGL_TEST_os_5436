#include <system/tasking/Process.h>

namespace hegel::tasking {

Process::~Process() {}

ErrorOr<size_t> Process::format(Stream& stream, FormatInfo& info)
{
    __unused(info);

    return hegel::format(stream, "{}({})", _name, _id);
}

}