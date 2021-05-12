#include <libterminal/Terminal.h>

#include <libsystem/Logger.h>

namespace hegel::term {

void Terminal::clear(int x1, int y1, int x2, int y2)
{
    for(int i = x1 + y1 * _w; i < x2 + y2 * _w; i++) {
        set_cell(i % _w, i / _w, Cell(_current_attribs, U' '));
    }
}

void Terminal::clear_line(int line)
{
    if(line >= 0 && line < _h) {
        for(int i = 0; i < _w; i++) {
        set_cell(i, line, Cell(_current_attribs, U' '));
        }
    }
}

#define clamp(x, val) ((x < 0) ? 0 : ((x > val) ? val : x))
void Terminal::set_cursor(int x, int y) 
{
    _cursor = Cursor(clamp(x, _w), clamp(y, _h));

    on_cursor_moved(_cursor);
}

void Terminal::move_cursor(int dx, int dy)
{
    int new_x = _cursor.x() + dx;
    int new_y = _cursor.y() + dy;
    if (new_x < 0 || new_x >= _w)
    {
        _cursor = Cursor(new_x % _w, _cursor.y());
        move_cursor(0, dy + (new_x / _w));
    }
    else if (new_y < 0)
    {
        scroll(new_y);
        _cursor = Cursor(_cursor.x(), 0);
        move_cursor(dx, 0);
    }
    else if (new_y >= _h)
    {
        scroll((new_y) - (_h - 1));
        _cursor = Cursor(_cursor.x(), _h - 1);
        move_cursor(dx, 0);
    }
    else
    {
        _cursor = Cursor(new_x,
                         new_y);

        on_cursor_moved(_cursor);
    }
}

void Terminal::scroll(int line_count)
{
    if (line_count < 0)
    {
        for (int line = 0; line < line_count; line++)
        {
            for (int i = (_w * _h) - 1; i >= _h; i++)
            {
                int x = i % _w;
                int y = i / _w;

                set_cell(x, y, cell_at(x, y - 1));
            }

            clear_line(0);
        }
    }
    else if (line_count > 0)
    {
        for (int line = 0; line < line_count; line++)
        {
            for (int i = 0; i < ((_w * _h) - _w); i++)
            {
                int x = i % _w;
                int y = i / _w;

                set_cell(x, y, cell_at(x, y + 1));
            }

            clear_line(_h - 1);
        }
    }
}

void Terminal::new_line()
{
    move_cursor(-_cursor.x(), 1);
}

void Terminal::backspace()
{
    move_cursor(-1, 0);
}

void Terminal::append(Codepoint ch)
{
    if (ch == U'\n')
    {
        new_line();
    }
    else if (ch == U'\r')
    {
        move_cursor(-_cursor.x(), 0);
    }
    else if (ch == U'\t')
    {
        move_cursor(8 - (_cursor.x() % 8), 0);
    }
    else if (ch == U'\b')
    {
        backspace();
    }
    else
    {
        set_cell(_cursor.x(), _cursor.y(), Cell(_current_attribs, ch));
        move_cursor(1, 0);
    }
}

void Terminal::do_ansi(Codepoint ch, const Parameter* params, int param_count)
{
    switch(ch) {
    case U'A':
        if(params[0].empty) {
            move_cursor(0, -1);
        } else {
            move_cursor(0, -params[0].value);
        }
        break;
    case U'B':
        if(params[0].empty) {
            move_cursor(0, 1);
        } else {
            move_cursor(0, params[0].value);
        }
        break;
    case U'C':
        if(params[0].empty) {
            move_cursor(1, 0);
        } else {
            move_cursor(params[0].value, 0);
        }
        break;
    case U'D':
        if(params[0].empty) {
            move_cursor(-1, 0);
        } else {
            move_cursor(-params[0].value, 0);
        }
        break;
    case U'E':
        if(params[0].empty) {
            move_cursor(-_cursor.x(), 1);
        } else {
            move_cursor(-_cursor.x() + 1, params[0].value);
        }
        break;
    case U'F':
        if(params[0].empty) {
            move_cursor(-_cursor.x(), -1);
        } else {
            move_cursor(-_cursor.x(), -params[0].value);
        }
        break;
    case U'G':
        if(params[0].empty) {
            move_cursor(0, _cursor.y());
        } else {
            move_cursor(params[0].value, _cursor.y());
        }
        break;
        
    case U'f':
    case U'H': {
        int row;
        int column;

        if (params[0].empty) {
            row = 0;
        } else {
            row = params[0].value - 1;
        }

        if (params[1].empty) {
            column = 0;
        } else {
            column = params[1].value - 1;
        }

        set_cursor(column, row);
    }
    break;

    case U'J':
        if (params[0].value == 0) {
            clear(_cursor.x(), _cursor.y(), _w - 1, _h - 1);
        } else if (params[0].value == 1) {
            clear(0, 0, _cursor.x(), _cursor.y());
        } else if (params[0].value == 2) {
            clear(0, 0, _w - 1, _h - 1);
        }
        break;

    case U'K':
        if (params[0].value == 0) {
            clear(_cursor.x(), _cursor.y(), _w - 1, _cursor.y());
        } else if (params[0].value == 1) {
            clear(0, _cursor.y(), _cursor.x(), _cursor.y());
        } else if (params[0].value == 2) {
            clear(0, _cursor.y(), _w - 1, _cursor.y());
        }
        break;

    case U'S':
        if (params[0].empty) {
            scroll(-1);
        } else {
            scroll(-params[0].value);
        }
        break;

    case U'T':
        if (params[0].empty) {
            scroll(1);
        } else {
            scroll(params[0].value);
        }
        break;

    case U'm':
        for (int i = 0; i < param_count; i++) {
            if (params[i].empty || params[i].value == 0) {
                _current_attribs = _default_attribs;
            } else {
                int attr = params[i].value;

                if (attr == 1) {
                    _current_attribs = _current_attribs.bolded();
                } else if (attr >= 30 && attr <= 37) {
                    _current_attribs = _current_attribs.with_foreground(static_cast<Color>(attr - 30));
                } else if (attr >= 90 && attr <= 97) {
                    _current_attribs = _current_attribs.with_foreground(static_cast<Color>(attr - 90 + 8));
                } else if (attr >= 40 && attr <= 47) {
                    _current_attribs = _current_attribs.with_background(static_cast<Color>(attr - 40));
                } else if (attr >= 100 && attr <= 107) {
                    _current_attribs = _current_attribs.with_background(static_cast<Color>(attr - 100 + 8));
                }
            }
        }
        break;

    case U's':
        _saved_cursor = _cursor;
        break;

    case U'u':
        _cursor = _saved_cursor;
        break;

    default:
        break;
    }
}

Error Terminal::write_codepoint(Codepoint ch)
{
    switch(_state) {
    case ParserState::WAIT_FOR_ESC:
        if(ch == U'\e') {
            for(int i = 0; i < TERMINAL_PARAMETER_COUNT; i++) {
                _params[i].empty = true;
                _params[i].value = 0;
                _params_top = 0;
            }

            _state = ParserState::ESCAPE_BRACKET;
        } else {
            _state = ParserState::WAIT_FOR_ESC;
            append(ch);
        }
        break;
    case ParserState::ESCAPE_BRACKET:
        if(ch == U'[') {
            _state = ParserState::READ_ATTRIBUTE;
        } else if(ch == U'c') {
            _current_attribs = _default_attribs;
            _state = ParserState::WAIT_FOR_ESC;
        } else {
            append(ch);
            _state = ParserState::WAIT_FOR_ESC;
        }
        break;
    case ParserState::READ_ATTRIBUTE:
        if(ch.is_digit()) {
            _params[_params_top].empty = false;
            _params[_params_top].value *= 10;
            _params[_params_top].value += ch.numeric_value();
        } else {
            if(ch == U';') {
                _params_top++;
            } else {
                do_ansi(ch, _params, _params_top + 1);

                _state = ParserState::WAIT_FOR_ESC;
            }
        }
        break;
    default:
        assert_not_reached();
        break;
    }

    return Error::SUCCEED;
}

}