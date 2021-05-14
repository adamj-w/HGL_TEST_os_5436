#pragma once

#include <libruntime/RefCounted.h>
#include <libruntime/RefPtr.h>
#include <libruntime/OwnPtr.h>
#include <libsystem/Formattable.h>

#include <kernel/tasking/Stack.h>
#include <kernel/tasking/Process.h>
#include <kernel/scheduling/Blocker.h>
#include <kernel/scheduling/Policy.h>

namespace hegel::tasking {

class Process;

typedef void (*ThreadEntry)(void);

#define THREAD_STATE_LIST(__ENTRY) \
    __ENTRY(EMBRYO) \
    __ENTRY(RUNNING) \
    __ENTRY(READY) \
    __ENTRY(BLOCKED) \
    __ENTRY(STOPPED) 

#define THREAD_STATE_ENUM_ENTRY(__x) __x,

enum class ThreadState
{
    THREAD_STATE_LIST(THREAD_STATE_ENUM_ENTRY) __COUNT
};

class Thread : public RefCounted<Thread>, public Formattable
{
private:
    int _id;
    ThreadEntry _entry;
    ThreadState _state;
    RefPtr<Process> _process;
    Stack _stack;
    Stack _userstack;

    OwnPtr<scheduling::Policy> _policy;
    OwnPtr<scheduling::Blocker> _blocker;

public:
    int id() const { return _id; }
    ThreadEntry entry() const { return _entry; }
    ThreadState state() const { return _state; }
    const char* state_string();
    void set_state(ThreadState state) { _state = state; }

    Stack& stack() { return _stack; }
    Stack& userstack() { return _userstack; }
    RefPtr<Process> process() { return _process; }
    Promotion promotion() { return _process->promotion(); }

    Thread(RefPtr<Process> process, ThreadEntry entry);
    virtual ~Thread();

    virtual void prepare() = 0;
    virtual void finalize() = 0;

    void start();
    void set_policy(OwnPtr<scheduling::Policy> policy);
    void block(OwnPtr<scheduling::Blocker> blocker);
    void switch_state(ThreadState new_state);

    bool should_unblock();
    void unblock();

    static RefPtr<Thread> create(RefPtr<Process> process, ThreadEntry entry);
    static void exit() __noreturn;
    static void sleep(unsigned long long milli);
    static void join(RefPtr<Thread> thread);
    static RefPtr<Thread> by_id(int id);

    static void cleanup(RefPtr<Thread> thread);
    static void foreach(Iteration (*callback)(RefPtr<Thread>));

    ErrorOr<size_t> format(Stream& stream, FormatInfo& info);
};

}