#pragma once

#include <libruntime/RefPtr.h>
#include <libsystem/Stream.h>

namespace hegel {

extern RefPtr<Stream> stdin;
extern RefPtr<Stream> stdout;
extern RefPtr<Stream> stderr;
extern RefPtr<Stream> stdlog;

template<typename... Args>
ErrorOrSizeT print(Args... args) {
    return format(*stdout, args...);
}

}