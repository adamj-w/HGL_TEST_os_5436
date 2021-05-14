#include <kernel/tasking/Thread.h>
#include <kernel/scheduling/Scheduling.h>
#include <kernel/scheduling/BlockerSleep.h>
#include <kernel/scheduling/BlockerJoin.h>

#include <libruntime/SpinLock.h>
#include <libsystem/Logger.h>

#include <arch/Arch.h>

using namespace hegel::scheduling;

namespace hegel::tasking {

static SpinLock _threads_lock;
static LinkedList<RefPtr<Thread>> _threads;
static volatile int _thread_id_counter;

Thread::Thread(RefPtr<Process> process, ThreadEntry entry)
    : _id(__atomic_add_fetch(&_thread_id_counter, 1, __ATOMIC_SEQ_CST))
    , _entry(entry)
    , _state(ThreadState::EMBRYO)
    , _process(process)
    , _stack(16)
    , _userstack(16) {}

Thread::~Thread()
{
    logger_info("Destructing {}...", this);
}

#define THREAD_STATE_STRING_ENTRY(__x) #__x,

static const char* _state_string[] = { THREAD_STATE_LIST(THREAD_STATE_STRING_ENTRY) };

const char* Thread::state_string()
{
    return _state_string[static_cast<int>(_state)];
}

void Thread::start()
{
    assert(_state == ThreadState::EMBRYO);

    finalize();

    switch_state(ThreadState::READY);
}

void Thread::block(OwnPtr<Blocker> blocker)
{
    _blocker = blocker;
    switch_state(ThreadState::BLOCKED);
    arch::yield();
}

void Thread::switch_state(ThreadState state)
{
    scheduling::update_thread_state(*this, state);
}

bool Thread::should_unblock()
{
    assert(_blocker);

    return _blocker->should_unblock();
}

void Thread::unblock()
{
    assert(_blocker);
    assert(_blocker->should_unblock());

    _blocker->unblock();
}

ErrorOr<size_t> Thread::format(Stream& stream, FormatInfo& info)
{
    __unused(info);

    return hegel::format(stream, "Thread(id={}, state={}, process={})", id(), state_string(), process());
}

RefPtr<Thread> Thread::create(RefPtr<Process> process, ThreadEntry entry)
{
    auto new_thread = arch::create_thread(process, entry);

    new_thread->prepare();

    _threads_lock.acquire();
    _threads.push_back(new_thread);
    _threads_lock.release();

    return new_thread;
}

void Thread::exit()
{
    logger_info("{} kill himself.", scheduling::running_thread());
    scheduling::update_thread_state(scheduling::running_thread(), ThreadState::STOPPED);
    arch::yield();
    assert_not_reached();
}

void Thread::sleep(unsigned long long time)
{
    if(time > 0) {
        logger_info("{} is going to sleep for {}ms", scheduling::running_thread(), time);
        scheduling::running_thread()->block(new scheduling::BlockerSleep(time));
    }
}

void Thread::join(RefPtr<Thread> thread_to_join)
{
    assert(thread_to_join != nullptr);

    if(thread_to_join->state() != ThreadState::STOPPED) {
        logger_info("{} is joining {}", scheduling::running_thread(), thread_to_join);
        scheduling::running_thread()->block(new BlockerJoin(thread_to_join));
    }
}

RefPtr<Thread> Thread::by_id(int id)
{
    RefPtr<Thread> result;

    _threads_lock.acquire();

    _threads.foreach([&result, id](RefPtr<Thread> thread) -> Iteration {
        if(thread->id() == id) {
            result = thread;
            return Iteration::STOP;
        }

        return Iteration::CONTINUE;
    });

    _threads_lock.release();

    return result;
}

void Thread::cleanup(RefPtr<Thread> thread)
{
    assert(thread != nullptr);

    _threads_lock.acquire();
    _threads.remove(thread);
    _threads_lock.release();
}

}