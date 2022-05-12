#pragma once

namespace proc {

class Blocker
{
public:
    Blocker() {}
    virtual ~Blocker() {}

    virtual bool should_unblock() = 0;
    virtual bool unblock() = 0;
};

}
