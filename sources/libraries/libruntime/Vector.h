#pragma once

#include <libruntime/Assert.h>
#include <libruntime/Move.h>

namespace hegel {

template<typename T>
class Vector
{
private:
    T* _storage;
    size_t _count;
    size_t _capacity;

public:
    size_t size() const { return _count; }
    bool empty() const { return _count == 0; }
    bool any() const { return !empty(); }

    Vector() : Vector(16) {}

    Vector(size_t capacity)
    {
        _count = 0;
        _capacity = capacity;
        _storage = new T[capacity];
    }

    ~Vector()
    {
        clear();

        delete _storage;
    }

    T& operator[](size_t index) {
        assert(_count > index);
        return _storage[index];
    }

    void clear()
    {
        for(size_t i = 0; i < _count; i++) {
            _storage[i].~T();
        }

        _count = 0;
    }

    void shrink()
    {
        _count--;

        if(_count < _capacity - _capacity / 4) {
            size_t new_cap = _capacity - _capacity / 4;
            T* buf = new T[new_cap];

            for(size_t i = 0; i < _count; ++i)
                buf[i] = move(_storage[i]);

            delete _storage;
            _storage = buf;
            _capacity = new_cap;
        }
    }

    void remove(size_t index) {
        assert(index < _count);
        _storage[index].~T();
        for(size_t i = index + 1; i < _count; ++i) {
            _storage[i-1] = move(_storage[i]);
        }
        shrink();
    }

    void remove(T& obj) {
        for(size_t i = 0; i < _count; ++i) {
            if(_storage[i] == obj) {
                remove(i);
                return;
            }
        }
    }
};

}