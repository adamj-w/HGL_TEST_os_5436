#include "Dispatcher.h"

#include "kernel/interrupts/Interrupts.h"
#include "kernel/scheduler/Blocker.h"
#include "kernel/system/System.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

static bool dispatcher_has_interrupt() {
    InterruptsRetainer retainer;

    for(size_t i = 0; i < 256; i++) {
        if(_pending_interrupts[i]) {
            return true;
        }
    }
    
    return false;
}

static void dispatcher_snapshot(bool* destination) {
    InterruptsRetainer retainer;
    memcpy(destination, _pending_interrupts, sizeof(_pending_interrupts));
    memset(_pending_interrupts, 0, sizeof(_pending_interrupts));
}

struct BlockerDispatcher : public sched::Blocker {
public:
    bool can_unblock(proc::Thread& t) override {
        __unused(t);
        return dispatcher_has_interrupt();
    }
};

static void dispatcher_dump(void* verbose) {
    InterruptsRetainer retainer;
    printf("--- Interrupt Dispatcher ---\n");

    uint8_t pending_count = 0;

    for(size_t i = 0; i < 256; ++i) {
        if(_pending_interrupts[i]) {
            pending_count++;
            if((bool)verbose == true) {
                printf("\t\t %x is held\n", i);
            }
        }
    }

    printf("\t %d total interrupts pending\n", pending_count);
}

void dispatcher_service() {
    while(true) {
        BlockerDispatcher blocker{};
        // TODO: scheduler yield this task

        while(dispatcher_has_interrupt()) {
            bool snapshot[256];
            dispatcher_snapshot(snapshot);

            for(size_t i = 0; i < 256; i++) {
                if(snapshot[i]) {
                    // TODO: dispatch interrupt
                }
            }
        }
    }
}

}
