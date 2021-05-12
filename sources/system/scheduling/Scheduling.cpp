#include "Scheduling.h"

#include <libruntime/LinkedList.h>
#include <libruntime/SpinLock.h>
#include <libsystem/Logger.h>

#include <libsystem/__plugs__.h>

#include <system/tasking/Thread.h>

using namespace hegel::tasking;

namespace hegel::scheduling {

static RefPtr<Thread> _running_thread;
static LinkedList<RefPtr<Thread>>* _ready_threads;
static LinkedList<RefPtr<Thread>>* _blocked_threads;

SpinLock _threads_lock;

void initialize()
{
    logger_info("Initializing scheduling");

    _ready_threads = new LinkedList<RefPtr<Thread>>();
    _blocked_threads = new LinkedList<RefPtr<Thread>>();
}

void update_thread_state(RefPtr<Thread> thread, ThreadState state)
{
    _threads_lock.acquire();

    if(thread->state() == ThreadState::EMBRYO) {
        if(_running_thread == nullptr) {
            logger_info("Using {} as running thread", thread);
            _running_thread = thread;
        }
    } else if(thread->state() == ThreadState::BLOCKED) {
        _blocked_threads->remove(thread);
    } else if(thread->state() == ThreadState::READY) {
        _ready_threads->remove(thread);
    }

    thread->set_state(state);

    if(thread->state() == ThreadState::BLOCKED) {
        _blocked_threads->push_back(thread);
    } else if(thread->state() == ThreadState::READY) {
         _ready_threads->push_back(thread);
    } else if(thread->state() == ThreadState::STOPPED) {}

    _threads_lock.release();
}

RefPtr<Thread> running_thread()
{
    assert(_running_thread != nullptr);

    return _running_thread;
}

bool can_schedule()
{
    return !plugs::memory_is_locked() && !_threads_lock.is_acquired();
}

static void unblock_blocked_thread()
{
    _blocked_threads->foreach([](RefPtr<Thread> thread) -> Iteration {
        if(thread->should_unblock()) {
            logger_info("Unblocking {}", thread);
            thread->unblock();
            thread->set_state(ThreadState::READY);
            _blocked_threads->remove(thread);
            _ready_threads->push_back(thread);
        }

        return Iteration::CONTINUE;
    });
}

uintptr_t schedule(uintptr_t esp)
{
    if(_running_thread == nullptr) {
        return esp;
    }

    if(can_schedule()) {
        _threads_lock.acquire();

        _running_thread->stack().set_pointer(esp);

        unblock_blocked_thread();

        if(_running_thread->state() == ThreadState::RUNNING) {
            _ready_threads->push_back(_running_thread);
            _running_thread->set_state(ThreadState::READY);
        }

        _running_thread = RefPtr<Thread>(*_ready_threads->pop());
        _running_thread->set_state(ThreadState::RUNNING);

        _threads_lock.release();

        return _running_thread->stack().get_pointer();
    } else {
        return esp;
    }
}

}