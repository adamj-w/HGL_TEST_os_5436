#include "Dispatcher.h"

#include "kernel/interrupts/Interrupts.h"
#include "kernel/system/System.h"

#include <stdio.h>
#include <stdint.h>

namespace interrupts {

static void dispatcher_dump(void* verbose);

static bool _pending_interrupts[256] = {};

void dispatcher_initialize() {
    // TODO: setup dispatcher thread
    hegel::register_panic_dump_callback(dispatcher_dump, (void*)true);
}

void dispatcher_dispatch(int intno) {
    _pending_interrupts[intno] = true;
    // TODO: device manage
}

static void dispatcher_dump(void* verbose) {
    InterruptsRetainer retainer;
    printf("--- Interrupt Dispatcher ---\n");

    uint8_t pending_count = 0;

    for(uint8_t i = 0; i < 255; ++i) {
        if(_pending_interrupts[i]) {
            pending_count++;
            if((bool)verbose == true) {
                printf("\t\t %x is held\n", i);
            }
        }
    }

    printf("\t %d total interrupts pending\n", pending_count);
}

}
