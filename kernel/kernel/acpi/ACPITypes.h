#pragma once

#include <libsystem/Types.h>
#include <libsystem/Macros.h>

namespace hegel::acpi {

struct __packed RSDPDescriptor 
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;

    bool validate() const __unchecked_all
    {
        uint8_t sum = 0;
        char* bytes = (char*)this;
        for(size_t i = 0; i < sizeof(RSDPDescriptor); i++) {
            sum += bytes[i];
        }
        return sum == 0;
    }
};

struct __packed RSDPDescriptor20 
{
    RSDPDescriptor v1;

    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
};


struct __packed SDTHeader 
{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;

    bool validate() __unchecked_all
    {
        uint8_t sum = 0;
        for(size_t i = 0; i < Length; i++) {
            sum += ((char*)this)[i];
        }
        return sum == 0;
    }
};

struct __packed RSDT
{
    struct SDTHeader Header;
    uint32_t NextSDT[];

    SDTHeader* child(size_t index)
    {
        return reinterpret_cast<SDTHeader*>(NextSDT[index]);
    }

    size_t child_count()
    {
        return (Header.Length - sizeof(RSDT::Header)) / 4;
    }
};

}