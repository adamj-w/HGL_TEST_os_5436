#pragma once

#include <libsystem/Format.h>
#include <libsystem/RefCounted.h>
#include <libsystem/OwnPtr.h>

#include <kernel/memory/AddressSpace.h>
#include <kernel/tasking/Promotion.h>

namespace hegel::tasking {

#define PROCESS_NAME_MAXLEN 256

class Process final : public Format, public RefCounted<Process>
{
private:
    int _id;
    char _name[PROCESS_NAME_MAXLEN];
    Promotion _promo = Promotion::SUPERVISOR;

    OwnPtr<memory::AddressSpace> _address_space;

public:
    int id() const {return _id;}
    const char* name() const { return _name; }
    Promotion promotion() const { return _promo; }
    memory::AddressSpace& address_space() { return *_address_space; }

    Process(const char* name);
    ~Process();

    //ErrorOrSizeT format(Stream& stream, FormatInfo& info);

};

}