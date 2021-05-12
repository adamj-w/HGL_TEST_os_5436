#pragma once

#include <system/scheduling/Blocker.h>
#include <system/tasking/Thread.h>

namespace hegel::scheduling {

class BlockerJoin final : public Blocker
{
private:
    RefPtr<tasking::Thread> _joined_thread;

public:
    BlockerJoin(RefPtr<tasking::Thread> thread_to_join) : _joined_thread(thread_to_join) {}

    ~BlockerJoin() {}

    bool should_unblock()
    {
        return _joined_thread->state() == tasking::ThreadState::STOPPED;
    }

    void unblock()
    {
        tasking::Thread::cleanup(_joined_thread);
        _joined_thread = nullptr;
    }
};

}