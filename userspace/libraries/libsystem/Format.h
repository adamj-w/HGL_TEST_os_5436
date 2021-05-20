#pragma once

#include <stdio.h>

class Format
{
public:
    virtual const char* format()
    {
        // TODO: memory leaking 19 ish bytes
        // "Object(0x00000000)" 18 char long
        char* str = new char[19];
        snprintf(str, 18, "Object(%#0X)");
        return str;
    }
    
    
};