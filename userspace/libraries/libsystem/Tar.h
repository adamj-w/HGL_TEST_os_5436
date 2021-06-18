#pragma once

#include <libsystem/Types.h>
#include <libsystem/ErrorOr.h>

class TarBlock 
{
public: 
    char name[100];
    uint32_t size;
    char* data;

    TarBlock(const char* name, uintptr_t data, size_t size);

    static ResultOr<TarBlock*> read(void* ramfile, size_t i);
};
