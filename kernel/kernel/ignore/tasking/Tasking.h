#pragma once

#include <libsystem/RefPtr.h>

namespace hegel::tasking {

class Process;

RefPtr<Process> kernel_process();

void initialize();

}