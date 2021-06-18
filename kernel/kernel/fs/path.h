#pragma once

#include <libsystem/Types.h>

namespace fs {

int path_read(const char* path, size_t index, char* buffer, size_t n);

void path_split(const char* path, char* dir, size_t dirN, char* file, size_t fileN);

}
