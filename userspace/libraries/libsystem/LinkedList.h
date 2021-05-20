#pragma once

#include <libsystem/Iteration.h>
//#include <libsystem/Callback.h>
#include <libsystem/Move.h>
#include <libsystem/Types.h>
#include <libsystem/Assert.h>

namespace hegel {

template<typename T>
struct LinkedListItem
{
    T value;

    LinkedListItem* prev;
    LinkedListItem* next;

    LinkedListItem(T value) : value(value), prev(nullptr), next(nullptr) {}
};

template<typename T>
class LinkedList
{
private:
    size_t _count;
    LinkedListItem<T>* _head;
    LinkedListItem<T>* _tail;

public:
    size_t count() const { return _count; }
    bool empty() const { return _count == 0; }
    bool any() const { return _count > 0; }

    LinkedList()
    {
        _count = 0;
        _head = nullptr;
        _tail = nullptr;
    }

    ~LinkedList()
    {
        clear();
    }

    void clear()
    {
        LinkedListItem<T>* current = _head;

        while(current) {
            LinkedListItem<T>* next = current->next;
            delete current;
            current = next;
        }

        _count = 0;
        _head = nullptr;
        _tail = nullptr;
    }

    template<typename Callback>
    void foreach(Callback callback)
    {
        LinkedListItem<T>* current = _head;

        while(current) {
            LinkedListItem<T>* next = current->next;

            if(callback(current->value) != Iteration::STOP) {
                current = next;
            } else 
                return;
        }
    }

    template <typename Comparator>
    void sort(Comparator comparator)
    {
        for (LinkedListItem<T> *i = _head; i->next; i = i->next) {
            for (LinkedListItem<T> *j = i->next; j; j = j->next) {
                if (comparator(i->value, j->value) > 0) {
                    T tmp = i->value;
                    i->value = j->value;
                    j->value = tmp;
                }
            }
        }
    }

    T push(T value) 
    {
        LinkedListItem<T>* item = new LinkedListItem<T>(move(value));
        item->prev = nullptr;
        if(!_head) {
            _head = item;
            _tail = item;
        } else {
            item->next = _head;
            _head->prev = item;
            _head = item;
        }

        _count++;
        return item->value;
    }

    T push_back(T value) 
    {
        LinkedListItem<T>* item = new LinkedListItem<T>(move(value));
        item->next = nullptr;
        if(!_tail) {
            _head = item;
            _tail = item;
        } else {
            item->prev = _tail;
            _tail->next = item;
            _tail = item;
        }

        _count++;
        return item->value;
    }

    T pop()
    {
        assert(_head);

        LinkedListItem<T>* item = _head;

        if(_count == 1) {
            _head = nullptr;
            _tail = nullptr;
        } else if(_count > 1) {
            item->next->prev = nullptr;
            _head = item->next;
        }

        _count--;
        
        T value = move(item->value);
        delete item;
        return value;
    }

    T pop_back()
    {
        assert(_tail);

        LinkedListItem<T>* item = _tail;

        if(_count == 1) {
            _head = nullptr;
            _tail = nullptr;
        } else if(_count > 1) {
            item->prev->next = nullptr;
            _tail = item->prev;
        }

        _count--;
        
        T value = move(item->value);
        delete item;
        return value;
    }

    T peek()
    {
        assert(_head);
        return _head->value;
    }

    T peek_back()
    {
        assert(_tail);
        return _tail->value;
    }

    T peek_and_pushback()
    {
        if(_count == 1) {
            return peek();
        } else {
            T value = peek();

            auto* item = _head;

            item->next->prev = nullptr;
            _head = item->next;

            item->next = nullptr;
            _tail->next = item;
            item->prev = _tail;
            _tail = item;

            return value;
        }
    }

    bool contains(T value)
    {
        bool ret = false;
        foreach([&](T i) -> Iteration {
            if(i == value) {
                ret = true;
                return Iteration::STOP;
            }
        });
        return ret;
    }

    void remove(T& value)
    {
        LinkedListItem<T>* cur = _head;

        while(cur) {
            LinkedListItem<T>* next = cur->next;

            if(cur->value == value) {
                if(cur->prev != nullptr) {
                    cur->prev->next = cur->next;
                } else {
                    _head = cur->next;
                }

                if(cur->next != nullptr) {
                    cur->next->prev = cur->prev;
                } else {
                    _tail = cur->prev;
                }

                _count--;
                delete cur;

                return;
            }

            cur = next;
        }
    }

    void remove_all(T& value)
    {
        LinkedListItem<T>* cur = _head;

        while(cur) {
            LinkedListItem<T>* next = cur->next;

            if(cur->value == value) {
                if(cur->prev != nullptr) {
                    cur->prev->next = cur->next;
                } else {
                    _head = cur->next;
                }

                if(cur->next != nullptr) {
                    cur->next->prev = cur->prev;
                } else {
                    _tail = cur->prev;
                }

                _count--;
                delete cur;
            }

            cur = next;
        }
    }
};

}