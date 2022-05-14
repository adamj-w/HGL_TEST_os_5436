#include "Interrupts.h"

#include <kernel/arch/Arch.h>

#include <libsystem/Logger.h>

namespace interrupts {

static bool _can_be_held = false;
static int _depth = 0;

void interrupts_initialize() {
    logger_info("Enabling interrupts!");

    interrupts_enable_holding();
    hegel::arch::enable_interrupts();
}

bool interrupts_retained() {
    return _can_be_held && _depth > 0;
}

void interrupts_enable_holding() {
    _can_be_held = true;
}

void interrupts_disable_holding() {
    _can_be_held = false;
}

void interrupts_retain() {
    if(_can_be_held) {
        hegel::arch::disable_interrupts();
        _depth++;
    }
}

void interrupts_release() {
    if(_can_be_held) {
        assert(_depth > 0);
        _depth--;

        if(_depth == 0) {
            hegel::arch::enable_interrupts();
        }
    }
}

}
