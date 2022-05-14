#pragma once

namespace interrupts {

typedef void (*InterruptHandler)();

void dispatcher_initialize();

void dispatcher_dispatch(int intno);

// TODO: void dispatcher_service();

}
