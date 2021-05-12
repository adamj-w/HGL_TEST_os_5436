#pragma once

namespace hegel::tasking {
class Thread;
}

namespace hegel::scheduling {

class Blocker
{
private:
public:
    Blocker() {}

    virtual ~Blocker() {}

    virtual bool should_unblock() = 0;
    virtual void unblock() = 0;
};

}