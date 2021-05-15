#pragma once

#include <libruntime/Macros.h>
#include <libterminal/Terminal.h>

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

class CGATerminal : public term::Terminal
{
private: 
    CGACell* _vga_cells;

public:
    CGATerminal(void* addr) 
        : hegel::term::Terminal(80, 25)
        , _vga_cells(reinterpret_cast<CGACell*>(addr))
    {
        _default_attribs = term::Attributes(false, term::Color::BLACK, term::Color::BRIGHT_GREY);
        _current_attribs = _default_attribs;

        for(int i = 0; i < _w * _h; i++) {
            on_cell_updated(i % _w, i / _w, term::Cell(_default_attribs, U' '));
        }

        enable_cursor();
        on_cursor_moved(term::Cursor(0, 0));
    }

    ~CGATerminal() {}

    void enable_cursor();
    void disable_cursor();

    void on_cell_updated(int x, int y, hegel::term::Cell c);
    void on_cursor_moved(hegel::term::Cursor cursor);
};

}