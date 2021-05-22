#include "Interrupts.h"
#include "PIC.h"
#include "../x86.h"

#include <libsystem/Logger.h>
#include <kernel/system/System.h>

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
    sti();
    logger_info("Interrupts should be working.");
}

static const char* __cpu_exception_string[] = {
    "Divide Error", "Debug", "NMI Interrupt", "Breakpoint", "Overflow", "Bound Range Exceeded",
    "Invalid Opcode", "Device Not Available (FPU)", "Double Fault", "CoProcessor Segment Overrun (FPU)",
    "Invalid TSS", "Segment Not Present", "Stack Segment Fault", "General Protection", "Page Fault",
    "Reserved", "FPU Error", "Alignment Check", "Machine Check", "SIMD FPU error", "Virtualization Exception",
    "Control Protection Exception"
};

extern "C" uint32_t interrupts_handler(uint32_t esp, InterruptStackFrame stackFrame)
{
    if(stackFrame.intno <= 21) {
        logger_error("CPU exception in process %d: %s IRQ%d!", 
            0, //scheduling::running_process()->id(),
            __cpu_exception_string[stackFrame.intno], stackFrame.intno);
    }

    if(stackFrame.intno == 32) {
        hegel::system_tick();
    }

    //logger_info("Executing intno: {}", stackFrame.intno);

    pic_ack(stackFrame.intno);

    return esp;
}

}