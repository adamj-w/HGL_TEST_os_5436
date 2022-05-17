#include "kernel/scheduler/Blocker.h"

#include "kernel/system/System.h"

namespace sched {

bool Blocker::has_timeout() const { return _timeout != (time_t)-1 && _timeout <= hegel::get_tick(); }

}