#include "PIC.h"
#include "../x86.h"

namespace hegel::arch::x86 {

#define PIC_WAIT()                    \
    do                                \
    {                                 \
        asm volatile("jmp 1f\n\t"     \
                     "1:\n\t"         \
                     "    jmp 2f\n\t" \
                     "2:");           \
    } while (0)

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void pic_remap(int offset1, int offset2)
{
    // ICW1
    out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    PIC_WAIT();
    out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    PIC_WAIT();

    // ICW2
    out8(PIC1_DATA, offset1);
    PIC_WAIT();
    out8(PIC2_DATA, offset2);
    PIC_WAIT();

    // ICW3
    out8(PIC1_DATA, 4);
    PIC_WAIT();
    out8(PIC2_DATA, 2);
    PIC_WAIT();

    // ICW4
    out8(PIC1_DATA, ICW4_8086);
    PIC_WAIT();
    out8(PIC2_DATA, ICW4_8086);
    PIC_WAIT();

    out8(PIC1_DATA, 0);
    PIC_WAIT();
    out8(PIC2_DATA, 0);
    PIC_WAIT();
}

void pic_ack(int intno)
{
    if(intno >= 40) {
        out8(0xA0, 0x20);
    }

    out8(0x20, 0x20);
}

}