#pragma once

#include <libsystem/Unicode.h>
#include "Attributes.h"

namespace hegel::term {

class Cell
{
private:
    Attributes _attribs;
    hegel::Codepoint _c;
    bool _dirty;

public:
    Attributes attributes() const { return _attribs; }
    hegel::Codepoint codepoint() const { return _c; }
    bool dirty() const { return _dirty; }

    Cell() : _attribs(), _c(' '), _dirty(true) {}
    Cell(Attributes attribs, char32_t c) : _attribs(attribs), _c(c), _dirty(true) {}

    Cell(Cell& other) : _attribs(other._attribs), _c(other._c), _dirty(other._dirty) {}
    Cell(Cell&& other) : _attribs(other._attribs), _c(other._c), _dirty(other._dirty) {}

    Cell& operator=(Cell& other) {
        _attribs = other._attribs;
        _c = other._c;
        _dirty = true;

        return *this;
    }

    Cell& operator=(Cell&& other) {
        _attribs = other._attribs;
        _c = other._c;
        _dirty = true;

        return *this;
    }

    bool operator==(Cell& other) {
        return other._c == _c && other._attribs == _attribs;
    }

    bool operator!=(Cell& other) {
        return !(*this == other);
    }
};

}