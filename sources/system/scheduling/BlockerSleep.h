#pragma once

#include <system/System.h>
#include <system/scheduling/Blocker.h>
#include <libruntime/Types.h>

namespace hegel::scheduling {

class BlockerSleep final : public Blocker
{
private:
    uint64_t _wakeup_tick;

public:
    BlockerSleep(uint64_t milli)
    {
        _wakeup_tick = get_tick() + milli;
    }

    ~BlockerSleep() {}

    bool should_unblock()
    {
        return _wakeup_tick <= get_tick();
    }

    void unblock()
    {
        
    }
};

}