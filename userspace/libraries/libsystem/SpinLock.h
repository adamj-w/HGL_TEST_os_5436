#pragma once

namespace hegel {

class SpinLock 
{
private:
    volatile bool _locked = false;

public:
    SpinLock() {}
    ~SpinLock() {}

    void acquire() 
    {
        while(!__sync_bool_compare_and_swap(&_locked, false, true)) {}

        __sync_synchronize();
    }

    void release()
    {
        __sync_synchronize();
        _locked = false;
    }

    bool is_acquired()
    {
        return _locked;
    }

    bool try_acquire()
    {
        if(__sync_bool_compare_and_swap(&_locked, 0, 1)) {
            __sync_synchronize();
            return true;
        } else {
            return false;
        }
    }
};

}