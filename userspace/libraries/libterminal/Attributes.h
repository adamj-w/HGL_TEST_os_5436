#pragma once

#include "Color.h"

namespace hegel::term {

class Attributes
{
private:
    bool _bold;

    Color _foreground;
    Color _background;

public:
    bool bold() const { return _bold; }
    Color foreground() const { return _foreground; }
    Color background() const { return _background; }

    Attributes()
        : _bold(false), _foreground(Color::GREY), _background(Color::BLACK) {}
    Attributes(bool bold, Color fore, Color back)
        : _bold(bold), _foreground(fore), _background(back) {}

    Attributes bolded() { return Attributes(true, _foreground, _background); }
    Attributes regular() { return Attributes(false, _foreground, _background); }
    Attributes with_foreground(Color fore) { return Attributes(_bold, fore, _background); }
    Attributes with_background(Color back) { return Attributes(_bold, _foreground, back); }

    bool operator==(Attributes& other) {
        return other._bold == _bold && 
            other._foreground == _foreground && 
            other._background == _background;
    }

    bool operator!=(Attributes& other) {
        return !(*this == other);
    }
};

}