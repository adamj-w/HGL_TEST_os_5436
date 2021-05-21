#include "PIT.h"
#include "../x86.h"

#include <libsystem/Macros.h>
#include <stdint.h>

using namespace hegel::arch::x86;

namespace PIT {

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_REG 0x43

#define CHANNEL(n) ((n & 0b11) << 6)
#define ACCESS_FULL_BYTE (0b11 << 4)
#define OPERATING_MODE(n) ((n & 0b111) << 1)

#define PIT_BASE_CLOCK (1193182)

void interrupt_on_frequency(int frequency) {
    uint16_t divisor = PIT_BASE_CLOCK / frequency;

    out8(PIT_MODE_REG, CHANNEL(0) | ACCESS_FULL_BYTE | OPERATING_MODE(0));
    out8(PIT_CHANNEL0, divisor & 0xFF);
    out8(PIT_CHANNEL0, (divisor >> 8) & 0xff);
}

}
