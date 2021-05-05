#pragma once

#include <libruntime/Error.h>

namespace hegel 
{

template<typename T>
class ErrorOr
{
private:
    Error _error;
    T _value;

public:
    bool succeed() { return _error == Error::SUCCEED; }
    T value() { return _value; }
    Error error() { return _error; }

    ErrorOr(Error error) : _error(error), _value() {}
    ErrorOr(T value) : _error(Error::SUCCEED), _value(value) {}
    ErrorOr(Error error, T value) : _error(error), _value(value) {}

    template<typename U>
    ErrorOr(ErrorOr<U> error, T value) : _error(error.error()), _value(value) {}

    bool operator==(Error err) {
        return _error == err;
    }

    bool operator!=(Error err) {
        return _error != err;
    }

    bool operator==(T value) {
        return _value == value;
    }

    bool operator!=(T value) {
        return _value != value;
    }
};

}