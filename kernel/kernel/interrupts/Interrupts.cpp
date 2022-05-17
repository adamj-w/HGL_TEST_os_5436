#include "Interrupts.h"

#include <kernel/arch/Arch.h>

#include <libsystem/Logger.h>

namespace interrupts {

static bool _holding_enabled = false;
static int _depth = 0;

void interrupts_initialize() {
    logger_info("Enabling interrupts!");

    interrupts_enable_holding();
    hegel::arch::enable_interrupts();
}

bool interrupts_retained() {
    return !_holding_enabled || _depth > 0;
}

void interrupts_enable_holding() {
    _holding_enabled = true;
}

void interrupts_disable_holding() {
    _holding_enabled = false;
}

void interrupts_retain() {
    if(_holding_enabled) {
        hegel::arch::disable_interrupts();
        _depth++;
    }
}

void interrupts_release() {
    if(_holding_enabled) {
        assert(_depth > 0);
        _depth--;

        if(_depth == 0) {
            hegel::arch::enable_interrupts();
        }
    }
}

}
