#pragma once

#include <libruntime/RefPtr.h>

namespace hegel::tasking {

class Process;

RefPtr<Process> kernel_process();

void initialize();

}