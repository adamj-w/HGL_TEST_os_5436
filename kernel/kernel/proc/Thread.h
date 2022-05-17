#pragma once

#include <kernel/scheduler/Blocker.h>

#include <libutil/OwnPtr.h>

#include <stdint.h>

namespace proc {

constexpr size_t PROCESS_NAME_SIZE = 128;
constexpr size_t PROCESS_STACK_SIZE = 65536;
#define PROCESS_ARG_COUNT 128
#define PROCESS_HANDLE_COUNT 128
#define PROCESS_SUCCESS (0)
#define PROCESS_FAILURE (1)

#define THREAD_STATE_LIST(__ITEM) \
    __ITEM(NONE) \
    __ITEM(HANG) \
    __ITEM(EMBRYO) \
    __ITEM(RUNNING) \
    __ITEM(BLOCKED) \
    __ITEM(CANCELED)

enum class ThreadState {
#define THREAD_STATE_ENUM_ENTRY(__state) __state,
    THREAD_STATE_LIST(THREAD_STATE_ENUM_ENTRY)
#undef THREAD_STATE_ENUM_ENTRY
    THREAD_STATE_COUNT
};
extern const char* thread_state_strings[];

#define THREAD_NONE (0)
#define THREAD_WAITABLE (1 << 0)
#define THREAD_USER (1 << 1)
typedef unsigned int ThreadFlags;

typedef void (*ThreadEntryPoint)();

class Thread {
public:
    Thread(Thread* parent, const char* name, ThreadFlags flags);
    ~Thread();
    __noncopyable(Thread);

    Thread* clone(uintptr_t sp, uintptr_t ip, ThreadFlags flags);
    static hegel::OwnPtr<Thread> spawn(Thread* parent, const char* name, ThreadEntryPoint entry, void* arg, ThreadFlags flags);

    void go();
    Result sleep(int timeout);
    // TODO: wait
    Result block(sched::Blocker& blocker, time_t timeout);
    void dump();

    void set_state(ThreadState state);
    void set_entry(ThreadEntryPoint entry) { entry_point = entry; }
    void try_unblock();

    uintptr_t kernel_stack_push(const void* value, size_t size);

    static size_t total();

    int id() const { return _id; }
    const char* name() const { return _name; }

    // Start (aka end it grows downward) of stack
    uintptr_t kernel_stack;
    // Location in the kernel stack
    uintptr_t kernel_stack_pointer;
    void* address_space;
    char fpu_registers[512];
    ThreadEntryPoint entry_point;

private:
    int _id;
    char _name[PROCESS_NAME_SIZE];
    ThreadFlags flags;

    ThreadState state;
    sched::Blocker* blocker;


    int exit_value = 0;

};

}
