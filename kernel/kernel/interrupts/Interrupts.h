#pragma once

#include <libsystem/Macros.h>
#include <libsystem/Assert.h>

#define ASSERT_INTERRUPTS_RETAINED() assert(interrupts::interrupts_retained())
#define ASSERT_INTERRUPTS_NOT_RETAINED() assert(!interrupts::interrupts_retained())

namespace interrupts {

void interrupts_initialize();

bool interrupts_retained();

void interrupts_enable_holding();
void interrupts_disable_holding();

void interrupts_retain();
void interrupts_release();

struct InterruptsRetainer
{
private:
    __noncopyable(InterruptsRetainer);
    __nonmovable(InterruptsRetainer);

public:
    InterruptsRetainer() {
        interrupts::interrupts_retain();
    }

    ~InterruptsRetainer() {
        interrupts::interrupts_release();
    }
};

}
