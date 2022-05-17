#pragma once

namespace interrupts {

typedef void (*InterruptHandler)();

void dispatcher_initialize();

void dispatcher_dispatch(int irq);

// TODO: void dispatcher_service();

}
