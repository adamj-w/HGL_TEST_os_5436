#pragma once

#include <libsystem/Formattable.h>
#include <libruntime/RefCounted.h>
#include <libruntime/OwnPtr.h>

#include <system/memory/AddressSpace.h>
#include <system/tasking/Promotion.h>

namespace hegel::tasking {

class Process final : public Formattable, public RefCounted<Process>
{
private:
    int _id;
    char _name[256];
    Promotion _promo = Promotion::SUPERVISOR;

    OwnPtr<memory::AddressSpace> _address_space;

public:
    int id() const {return _id;}
    const char* name() const { return _name; }
    Promotion promotion() const { return _promo; }
    memory::AddressSpace& address_space() { return *_address_space; }

    Process(const char* name);
    ~Process();

    ErrorOr<size_t> format(Stream& stream, FormatInfo& info);

};

}