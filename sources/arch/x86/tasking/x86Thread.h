#pragma once

#include "../interrupts/Interrupts.h"
#include <system/tasking/Process.h>
#include <system/tasking/Thread.h>

namespace hegel::arch::x86 {

class x86Thread final : public tasking::Thread
{
public:
    x86Thread(RefPtr<tasking::Process> process, tasking::ThreadEntry entry) : Thread(process, entry) {}

    ~x86Thread() {}

    void prepare()
    {
        stack().push(0);
        stack().push(0);

        userstack().push(0);
        userstack().push(0);
    }

    void finalize()
    {
        InterruptStackFrame frame;

        if(promotion() == tasking::Promotion::USER) {
            stack().push((uint32_t)0x20);
            stack().push(userstack().get_pointer());
            stack().push((uint32_t)0x202);

            frame.cs = 0x18;
            frame.ds = 0x20;
            frame.es = 0x20;
            frame.fs = 0x20;
            frame.gs = 0x20;
        } else {
            frame.cs = 0x08;
            frame.ds = 0x10;
            frame.es = 0x10;
            frame.fs = 0x10;
            frame.gs = 0x10;
        }

        frame.ebp = stack().get_pointer();
        frame.esp = stack().get_pointer();
        frame.eip = (uint32_t)entry();
        frame.eflags = 0x202;

        stack().push(&frame);
    }
};

}