#pragma once

#include <libruntime/Error.h>
#include <stddef.h>

namespace hegel 
{

template<typename T>
class ErrorOr
{
protected:
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

class ErrorOrSizeT : public ErrorOr<size_t>
{
public:
    size_t unwrap() { return succeed() ? _value : -1; }

    ErrorOrSizeT(Error error) : ErrorOr(error) {}
    ErrorOrSizeT(size_t value) : ErrorOr(value) {}
    ErrorOrSizeT(Error error, size_t value) : ErrorOr(error, value) {}

    template<typename U>
    ErrorOrSizeT(ErrorOr<U> error, size_t value) : ErrorOr(error, value) {}
};

}