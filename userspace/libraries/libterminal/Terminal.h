#pragma once

#include "Cell.h"

#include <libsystem/Unicode.h>

namespace hegel::term {

#define TERMINAL_PARAMETER_COUNT 8

enum class ParserState
{
    WAIT_FOR_ESC,
    ESCAPE_BRACKET,
    READ_ATTRIBUTE,
};

class Cursor
{
private:
    int _x;
    int _y;
    
public:
    int x() const { return _x; }
    int y() const { return _y; }

    Cursor() : _x(0), _y(0) {}
    Cursor(int x, int y) : _x(x), _y(y) {}
};

struct Parameter
{
    int value;
    bool empty;
};

class Terminal : public hegel::UTF8Stream
{
protected:
    Cursor _cursor;
    Cursor _saved_cursor;

    int _w, _h;
    Cell* _cells;

    Attributes _default_attribs, _current_attribs;

    Parameter _params[TERMINAL_PARAMETER_COUNT];
    int _params_top;

    ParserState _state;
public:
    int width() const { return _w; }
    int height() const { return _h; }

    Cell cell_at(int x, int y) const {
        return _cells[y * _w + x];
    }

    void set_cell(int x, int y, Cell c) {
        if(x >= 0 && x < _w && y >= 0 && y < _h) {
            if(_cells[y * _w + x] != c) {
                _cells[y * _w + x] = c;
                on_cell_updated(x, y, c);
            }
        }
    }

    Terminal(int width, int height)
        : _w(width), _h(height), _cells(new Cell[width * height]), 
        _default_attribs(), _current_attribs(),
        _state(ParserState::WAIT_FOR_ESC)
    {}

    ~Terminal() {}

    void clear() { clear(0, 0, _w, _h); }
    void clear(int x1, int y1, int x2, int y2);
    void clear_line(int line);

    void set_cursor(int x, int y);
    void move_cursor(int dx, int dy);

    void scroll(int num_lines);
    void new_line();

    void backspace();
    void append(Codepoint ch);

    void do_ansi(Codepoint ch, const Parameter* params, int param_count);
    virtual Result write_codepoint(Codepoint ch);

    virtual void on_cell_updated(int x, int y, Cell c) = 0;
    virtual void on_cursor_moved(Cursor cursor) = 0;
};

}