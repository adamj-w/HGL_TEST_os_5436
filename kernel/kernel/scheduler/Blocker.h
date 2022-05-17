#pragma once

#include <libsystem/Error.h>
#include <libsystem/Macros.h>
#include <time.h>

namespace proc {
    class Thread;
}

namespace sched {

struct Blocker {
private:
    Result _result = Result::SUCCEED;
    time_t _timeout = -1;
    bool _interrupted = false;

public:
    Result result() const { return _result; }

    void timeout(time_t ts) { _timeout = ts; }

    virtual ~Blocker() {}

    void unblock(proc::Thread& t) {
        _result = Result::SUCCEED;
        on_unblock(t);
    }

    void timeout(proc::Thread& t) {
        _result = Result::TIMEOUT;
        on_timeout(t);
    }

    void interrupt(proc::Thread& t, Result result) {
        _interrupted = true;
        _result = result;
        on_interrupt(t);
    }

    bool has_timeout() const;
    bool is_interrupted() const { return _interrupted; }

    virtual bool can_unblock(proc::Thread& t) { __unused(t); return true; }
    virtual void on_unblock(proc::Thread& t) { __unused(t); }
    virtual void on_timeout(proc::Thread& t) { __unused(t); }
    virtual void on_interrupt(proc::Thread& t) { __unused(t); }
};

}
