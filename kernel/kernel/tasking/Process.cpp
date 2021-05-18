#include <kernel/tasking/Process.h>

#include <libruntime/SpinLock.h>
#include <libsystem/Logger.h>

#include <libc/string.h>

namespace hegel::tasking {

static int _process_id_counter = -1;
static SpinLock _processes_lock;
static LinkedList<RefPtr<Process>> _processes;

Process::Process(const char* str)
    : _id(__atomic_add_fetch(&_process_id_counter, 1, __ATOMIC_SEQ_CST))
{
    size_t len = strlen(str);
    if(len >= PROCESS_NAME_MAXLEN) {
        logger_warn("Process constructed with length {d} but max length is {d}.", len, PROCESS_NAME_MAXLEN);
    }

    strncpy(_name, str, PROCESS_NAME_MAXLEN - 1);
}

Process::~Process() {}

ErrorOrSizeT Process::format(Stream& stream, FormatInfo& info)
{
    __unused(info);

    return hegel::format(stream, "{}({})", _name, _id);
}

}