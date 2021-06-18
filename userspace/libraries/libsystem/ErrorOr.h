#pragma once

#include <libsystem/Error.h>
#include <stddef.h>

template<typename T>
class ResultOr
{
protected:
    Result _error;
    T _value;

public:
    bool succeed() { return _error == Result::SUCCEED; }
    T value() { return _value; }
    Result error() { return _error; }

    ResultOr(Result error) : _error(error), _value() {}
    ResultOr(T value) : _error(Result::SUCCEED), _value(value) {}
    ResultOr(Result error, T value) : _error(error), _value(value) {}

    template<typename U>
    ResultOr(ResultOr<U> error, T value) : _error(error.error()), _value(value) {}

    bool operator==(Result err) {
        return _error == err;
    }

    bool operator!=(Result err) {
        return _error != err;
    }

    bool operator==(T value) {
        return _value == value;
    }

    bool operator!=(T value) {
        return _value != value;
    }
};

class ErrorOrSizeT : public ResultOr<size_t>
{
public:
    size_t unwrap() { return succeed() ? _value : -1; }

    ErrorOrSizeT(Result error) : ResultOr(error) {}
    ErrorOrSizeT(size_t value) : ResultOr(value) {}
    ErrorOrSizeT(Result error, size_t value) : ResultOr(error, value) {}

    template<typename U>
    ErrorOrSizeT(ResultOr<U> error, size_t value) : ResultOr(error, value) {}
};
