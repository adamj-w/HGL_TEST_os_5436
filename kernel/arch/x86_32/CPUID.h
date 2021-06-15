#pragma once

#include <libsystem/Types.h>

namespace hegel::arch::x86 {

extern "C" uint32_t cpuid_supported(void);

extern "C" uint32_t cpuid_get_features(void);
extern "C" uint32_t cpuid_vender_code(void);

}
