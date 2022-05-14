#include "Interrupts.h"
#include "PIC.h"
#include "../x86.h"
#include "../segmentation/Segmentation.h"

#include <libsystem/Logger.h>
#include <kernel/system/System.h>
#include <kernel/interrupts/Interrupts.h>

#include "arch/Arch.h"

namespace hegel::arch::x86 {

IDTEntry idt_entries[IDT_ENTRY_COUNT] = {};
IDTDescriptor idt_descriptor = {};

extern "C" uintptr_t __interrupt_vector[];

void interrupts_initialize()
{
    logger_info("Initializing system interrupts.");

    pic_remap(0x20, 0x28);

    for(size_t i = 0; i < 32; i++) {
        idt_entries[i] = IDTEntry::create(__interrupt_vector[i], 0x08, IDT_INTGATE);
    }

    for(size_t i = 32; i < 48; i++) {
        idt_entries[i] = IDTEntry::create(__interrupt_vector[i], 0x08, IDT_TRAPGATE);
    }

    idt_entries[128] = IDTEntry::create(__interrupt_vector[48], 0x08, IDT_TRAPGATE);

    idt_descriptor.offset = (uintptr_t)&idt_entries[0];
    idt_descriptor.size = sizeof(IDTEntry) * IDT_ENTRY_COUNT;

    logger_info("Loading the IDT.");
    x86::load_idt((uint32_t)&idt_descriptor);
}

static const char* __cpu_exception_string[] = {
    "Divide Error", "Debug", "NMI Interrupt", "Breakpoint", "Overflow", "Bound Range Exceeded",
    "Invalid Opcode", "Device Not Available (FPU)", "Double Fault", "CoProcessor Segment Overrun (FPU)",
    "Invalid TSS", "Segment Not Present", "Stack Segment Fault", "General Protection", "Page Fault",
    "Reserved", "FPU Error", "Alignment Check", "Machine Check", "SIMD FPU error", "Virtualization Exception",
    "Control Protection Exception", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", 
    "Reserved", "Reserved", "Reserved", "Reserved"
};

extern "C" uint32_t interrupts_handler(uint32_t esp, InterruptStackFrame stackFrame)
{
    ASSERT_INTERRUPTS_NOT_RETAINED(); // no manual calling

    // TODO: optimize control block placement
    if(stackFrame.intno < 32) {
        if(stackFrame.cs == HEGEL_PROCESS_CODE_SEG) {
            logger_error("CPU exception in process %d: %s (IRQ%d)!", 
                0, //scheduling::running_process()->id(),
                __cpu_exception_string[stackFrame.intno], stackFrame.intno);
        } else {
            logger_error("CPU exception in kernel: %s (IRQ%d)!",
                __cpu_exception_string[stackFrame.intno], stackFrame.intno);

            PANIC("CPU exception in kernel (IRQ%d): %s!\n", stackFrame.intno, __cpu_exception_string[stackFrame.intno]);
        }
    } else if (stackFrame.intno < 48) {
        interrupts::interrupts_disable_holding();

        int irq = stackFrame.intno - 32;

        if(irq == 0) {
            hegel::system_tick();
            // TODO: scheduler
        } else {
            // TODO: use interrupt dispatcher
        }

        interrupts::interrupts_enable_holding();
    } else if(stackFrame.intno == 127) {
        // yield
        interrupts::interrupts_disable_holding();

        // TODO: schedule yield

        interrupts::interrupts_enable_holding();
    } else if(stackFrame.intno == 128) {
        // syscall
        x86::sti();

        logger_debug("syscall");

        x86::cli();
    }

    pic_ack(stackFrame.intno);

    return esp;
}

}