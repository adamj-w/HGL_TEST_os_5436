#include "CGATerminal.h"

#include "../x86.h"

namespace hegel::arch::x86 {

CGAColor terminal_color_to_cga(hegel::term::Color color)
{
    switch (color)
    {
    case hegel::term::Color::BLACK:
        return CGAColor::BLACK;

    case hegel::term::Color::RED:
        return CGAColor::RED;

    case hegel::term::Color::GREEN:
        return CGAColor::GREEN;

    case hegel::term::Color::YELLOW:
        return CGAColor::BROWN;

    case hegel::term::Color::BLUE:
        return CGAColor::BLUE;

    case hegel::term::Color::MAGENTA:
        return CGAColor::MAGENTA;

    case hegel::term::Color::CYAN:
        return CGAColor::CYAN;

    case hegel::term::Color::GREY:
        return CGAColor::GREY;

    case hegel::term::Color::BRIGHT_BLACK:
        return CGAColor::BRIGHT_BLACK;

    case hegel::term::Color::BRIGHT_RED:
        return CGAColor::BRIGHT_RED;

    case hegel::term::Color::BRIGHT_GREEN:
        return CGAColor::BRIGHT_GREEN;

    case hegel::term::Color::BRIGHT_YELLOW:
        return CGAColor::BRIGHT_BROWN;

    case hegel::term::Color::BRIGHT_BLUE:
        return CGAColor::BRIGHT_BLUE;

    case hegel::term::Color::BRIGHT_MAGENTA:
        return CGAColor::BRIGHT_MAGENTA;

    case hegel::term::Color::BRIGHT_CYAN:
        return CGAColor::BRIGHT_CYAN;

    case hegel::term::Color::BRIGHT_GREY:
        return CGAColor::BRIGHT_GREY;

    default:
        return CGAColor::MAGENTA;
    }
}

void CGATerminal::clear_memory()
{
    for(int i = 0; i < _w * _h; ++i) {
        _vga_cells[i].clear();
    }
}

void CGATerminal::enable_cursor()
{
    out8(0x3D4, 0x0A);
    out8(0x3D5, (in8(0x3D5) & 0xC0) | 0);
    out8(0x3D4, 0x0B);
    out8(0x3D5, (in8(0x3D5) & 0xE0) | 15);
}

void CGATerminal::disable_cursor()
{
    out8(0x3D4, 0x0A);
    out8(0x3D5, 0x20);
}

void CGATerminal::on_cell_updated(int x, int y, hegel::term::Cell c)
{
    _vga_cells[y * _w + x] = CGACell(c.codepoint(), 
        terminal_color_to_cga(c.attributes().foreground()),
        terminal_color_to_cga(c.attributes().background()));
}

void CGATerminal::on_cursor_moved(hegel::term::Cursor cursor)
{
    uint16_t pos = cursor.y() * _w + cursor.x();

    out8(0x3DA, 0x0F);
    out8(0x3D5, (uint8_t)(pos & 0xFF));
    out8(0x3D4, 0x0E);
    out8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

}