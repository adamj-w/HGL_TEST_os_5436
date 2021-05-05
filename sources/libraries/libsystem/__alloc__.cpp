#include <libc/string.h>
#include <libruntime/Assert.h>
#include <libsystem/__alloc__.h>
#include <libsystem/__plugs__.h>

using namespace hegel;

#define VERSION "1.1"
#define ALIGNMENT 16ul

#define ALIGN_TYPE char
#define ALIGN_INFO sizeof(ALIGN_TYPE) * 16

#define ALIGN(ptr) \
    if(ALIGNMENT > 1) { \
        uintptr_t diff; \
        ptr = (void*)((uintptr_t)ptr + ALIGN_INFO); \
        diff = (uintptr_t)ptr & (ALIGNMENT - 1); \
        if(diff != 0) { \
            diff = ALIGNMENT - diff; \
            ptr = (void*)((uintptr_t)ptr + diff); \
        } \
        *((ALIGN_TYPE*)((uintptr_t)ptr - ALIGN_INFO)) = \
            diff + ALIGN_INFO; \
    }

#define UNALIGN(ptr) \
    if(ALIGNMENT > 1) { \
        uintptr_t diff = *((ALIGN_TYPE*)((uintptr_t)ptr - ALIGN_INFO)); \
        if(diff < (ALIGNMENT + ALIGN_INFO)) { \
            ptr = (void*)((uintptr_t)ptr - diff); \
        } \
    }

#define LIBALLOC_MAGIC 0xC001C0DE
#define LIBALLOC_DEAD 0xDEADBEEF

struct AllocMinor;

struct AllocMajor
{
    unsigned int pages;
    unsigned int size;
    unsigned int usage;
    AllocMajor* prev;
    AllocMajor* next;
    AllocMinor* first;
};

struct AllocMinor 
{
    unsigned int magic;
    unsigned int size;
    unsigned int req_size;
    AllocMinor* prev;
    AllocMinor* next;
    AllocMajor* block;
};

static AllocMajor* l_memRoot = nullptr;
static AllocMajor* l_bestBet = nullptr;

static unsigned int l_pageCount = 16;
static unsigned long long l_allocated = 0;
static unsigned long long l_inuse = 0;

static long long l_warningCount = 0;
//static long long l_errorCount = 0;
//static long long l_possibleOverruns = 0;

/*static void liballoc_dump()
{
    //AllocMajor* maj = l_memRoot;
    //AllocMinor* min = nullptr;

    // TODO: printf all the information
}*/

static AllocMajor* allocate_new_page(unsigned int size)
{
    unsigned int st;
    AllocMajor* maj;

    st = size + sizeof(AllocMajor);
    st += sizeof(AllocMinor);
    
    if((st % plugs::get_page_size()) == 0)
        st = st / (plugs::get_page_size());
    else
        st = st / (plugs::get_page_size()) + 1;

    if(st < l_pageCount)
        st = l_pageCount;

    auto result = plugs::memory_alloc(st);

    if(result.error() != Error::SUCCEED) {
        l_warningCount++;
        //logger_warn("plugs::memory_alloc({}) return nullptr", st);
        return nullptr;
    } else {
        maj = (AllocMajor*)result.value();
    }

    maj->prev = nullptr;
    maj->next = nullptr;
    maj->pages = st;
    maj->size = st * plugs::get_page_size();
    maj->usage = sizeof(AllocMajor);
    maj->first = nullptr;

    l_allocated += maj->size;

    return maj;
}

namespace hegel {
namespace __alloc__ {

extern "C" void* malloc(size_t req_size)
{
    int startedBet = 0;
    unsigned long long bestSize = 0;
    void* p = nullptr;
    uintptr_t diff;
    AllocMajor* maj;
    AllocMinor* min;
    AllocMinor* new_min;
    unsigned long size = req_size;

    if(ALIGNMENT > 1) {
        size += ALIGNMENT + ALIGN_INFO;
    }

    plugs::memory_lock();

    if(size == 0) {
        l_warningCount++;
        // TODO: warn about malloc
        plugs::memory_unlock();
        return malloc(1);
    }

    if(l_memRoot == nullptr) {
        l_memRoot = allocate_new_page(size);

        if(l_memRoot == nullptr) {
            plugs::memory_unlock();
            // TODO: error message
            assert_not_reached();
        }
    }

    maj = l_memRoot;
    startedBet = 0;

    if(l_bestBet != nullptr) {
        bestSize = l_bestBet->size - l_bestBet->usage;
        if(bestSize > (size + sizeof(AllocMinor))) {
            maj = l_bestBet;
            startedBet = 1;
        }
    }

    while(maj != nullptr) {
        diff = maj->size - maj->usage;

        if(bestSize < diff) {
            l_bestBet = maj;
            bestSize = diff;
        }

        if(diff < (size + sizeof(AllocMinor))) {
            if(maj->next != nullptr) {
                maj = maj->next;
                continue;
            }

            if(startedBet == 1) {
                maj = l_memRoot;
                startedBet = 0;
                continue;
            }

            maj->next = allocate_new_page(size);
            if(maj->next == nullptr)
                break;
            maj->next->prev = maj;
            maj = maj->next;
        }

        if(maj->first == nullptr) {
            maj->first = (AllocMinor*)((uintptr_t)maj + sizeof(AllocMajor));

            maj->first->magic = LIBALLOC_MAGIC;
            maj->first->prev = nullptr;
            maj->first->next = nullptr;
            maj->first->block = maj;
            maj->first->size = size;
            maj->first->req_size = req_size;
            maj->usage += size + sizeof(AllocMinor);

            l_inuse += size;

            p = (void*)((uintptr_t)(maj->first) + sizeof(AllocMinor));

            ALIGN(p);

            plugs::memory_unlock();
            return p;
        }

        diff = (uintptr_t)(maj->first);
        diff -= (uintptr_t)maj;
        diff -= sizeof(AllocMajor);

        if(diff >= (size + sizeof(AllocMinor))) {
            maj->first->prev = (AllocMinor*)((uintptr_t)maj + sizeof(AllocMajor));
            maj->first->prev->next = maj->first;
            maj->first = maj->first->prev;

            maj->first->magic = LIBALLOC_MAGIC;
            maj->first->prev = nullptr;
            maj->first->block = maj;
            maj->first->size = size;
            maj->first->req_size = req_size;
            maj->usage += size + sizeof(AllocMinor);

            l_inuse += size;

            p = (void*)((uintptr_t)(maj->first) + sizeof(AllocMinor));
            ALIGN(p);

            plugs::memory_unlock();
            return p;
        }

        min = maj->first;

        while(min != nullptr) {
            if(min->next == nullptr) {
                diff = (uintptr_t)(maj) + maj->size;
                diff -= (uintptr_t)min;
                diff -= sizeof(AllocMinor);
                diff -= min->size;

                if(diff >= (size + sizeof(AllocMinor))) {
                    min->next = (AllocMinor *)((uintptr_t)min + sizeof(AllocMinor) + min->size);
                    min->next->prev = min;
                    min = min->next;
                    min->next = nullptr;
                    min->magic = LIBALLOC_MAGIC;
                    min->block = maj;
                    min->size = size;
                    min->req_size = req_size;
                    maj->usage += size + sizeof(AllocMinor);

                    l_inuse += size;

                    p = (void*)((uintptr_t)min + sizeof(AllocMinor));
                    ALIGN(p);

                    plugs::memory_unlock();
                    return p;
                }
            } else {
                diff = (uintptr_t)(min->next);
                diff -= (uintptr_t)min;
                diff -= sizeof(AllocMinor);
                diff -= min->size;

                if(diff >= (size + sizeof(AllocMinor))) {
                    new_min = (AllocMinor*)((uintptr_t)min + sizeof(AllocMinor) + min->size);

                    new_min->magic = LIBALLOC_MAGIC;
                    new_min->next = min->next;
                    new_min->prev = min;
                    new_min->size = size;
                    new_min->req_size = req_size;
                    new_min->block = maj;
                    min->next->prev = new_min;
                    min->next = new_min;
                    maj->usage += size + sizeof(AllocMinor);

                    l_inuse += size;

                    p = (void*)((uintptr_t)new_min + sizeof(AllocMinor));
                    ALIGN(p);

                    plugs::memory_unlock();
                    return p;
                }
            }

            min = min->next;
        }

        if(maj->next == nullptr) {
            if(startedBet == 1) {
                maj = l_memRoot;
                startedBet = 0;
                continue;
            }

            maj->next = allocate_new_page(size);
            if(maj->next == nullptr)
                break;
            maj->next->prev = maj;
        }

        maj = maj->next;
    }

    plugs::memory_unlock();

    // TODO: log fatal out of memory
    assert_not_reached();

    return nullptr;
}

}
}
