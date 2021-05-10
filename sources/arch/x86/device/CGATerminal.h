#pragma once

#include <libruntime/Macros.h>

namespace hegel::arch::x86 {

enum CGAColor
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,

    BRIGHT_BLACK,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    BRIGHT_BROWN,
    BRIGHT_GREY,
};

struct __packed CGACell
{
public:
    char codepoint;
    CGAColor foreground : 4;
    CGAColor background : 4;

    CGACell() {}
    CGACell(char codepoint, CGAColor foreground, CGAColor background) 
        : codepoint(codepoint), foreground(foreground), background(background) {}

    void clear() {
        codepoint = ' ';
    }
};

// TODO: make this a terminal
class CGATerminal
{
private: 
    CGACell* _cells;

public:
    CGATerminal(void* addr) : _cells(reinterpret_cast<CGACell*>(addr)) {
        clear_memory();
        enable_cursor();
    }
};

}