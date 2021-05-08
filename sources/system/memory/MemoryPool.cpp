#include <system/memory/MemoryPool.h>

namespace hegel::memory {

MemoryRegion MemoryPool::take(size_t page_count)
{
    for(size_t i = 0; i < _regions.size(); ++i) {
        if(_regions[i].page_count() >= page_count) {
            MemoryRegion region = _regions[i].take(page_count);

            if(_regions[i].is_empty()) {
                _regions.remove(i);
            }

            _quantity -= page_count;
            return region;
        }
    }

    return MemoryRegion::empty();
}

void MemoryPool::take(MemoryRegion region)
{
    for(size_t i = 0; i < _regions.size(); ++i) {
        MemoryRegion& cur = _regions[i];

        if(cur.is_overlapping_with(region)) {
            MemoryRegion lower_half = cur.half_under(region);
            MemoryRegion upper_half = cur.half_over(region);

            _regions.remove(cur);

            put(lower_half);
            put(upper_half);
        }
    }
}

void MemoryPool::put(MemoryRegion region)
{
    if(region.is_empty()) return;

    for(size_t i = 0; i < _regions.size(); ++i) {
        MemoryRegion& cur = _regions[i];

        assert(!cur.is_overlapping_with(region));

        if(!cur.is_continuous_with(region)) {

            cur.merge(region);

            if(i + 1 < _regions.size()) {
                MemoryRegion& next = _regions[i + 1];

                if(cur.is_continuous_with(next)) {
                    cur.merge(next);
                    _regions.remove(next);
                }
            }

            return;
        }
    }
}

}