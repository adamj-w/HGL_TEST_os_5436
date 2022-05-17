#pragma once

#include <libsystem/Types.h>
#include <libsystem/Macros.h>
#include <libutil/Iteration.h>
#include <libsystem/Logger.h>

#include <string.h>

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
    uint32_t children[];

    const SDTHeader* child(size_t index) const {
        return reinterpret_cast<SDTHeader*>(children[index]);
    }

    size_t child_count() const {
        return (Header.Length - sizeof(RSDT::Header)) / 4;
    }

    void print_children() const {
        for(size_t i = 0; i < child_count(); i++) {
            auto* sdt = child(i);
            char str[5];
            memcpy(str, sdt->Signature, 4);
            str[4] = '\0';
            logger_info("Found SDT with Signature %s at %#010X", str, sdt);
        }
    }

    const SDTHeader* child(const char* signature) const {
        for(size_t i = 0; i < child_count(); i++) {
            const SDTHeader* sdth = child(i);

            if(memcmp(sdth->Signature, signature, 4) == 0) {
                return sdth;
            }
        }

        return nullptr;
    }
};

enum class MADT_RecordType : uint8_t
{
    LAPIC = 0,
    IOAPIC = 1,
    ISO = 2,
    NMI = 4,
    LAPIC_OVERRIDE = 5,
};

struct __packed MADT_Record 
{
    MADT_RecordType type;
    uint8_t length;
};

struct __packed MADT_IO_ACPI_Record
{
    MADT_Record header;
    uint8_t id;
    uint8_t reserved;
    uint32_t address;
    uint32_t interrupt_base;
};

struct __packed MADT
{
    struct SDTHeader* header;

    uint32_t local_apic;
    uint32_t flags;
};

}