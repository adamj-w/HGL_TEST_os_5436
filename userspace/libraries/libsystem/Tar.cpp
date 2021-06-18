#include "Tar.h"
#include "Macros.h"
#include "Assert.h"

#include <string.h>

struct __packed tar_rawheader
{                              /* byte offset */
  char name[100];               /*   0 */
  char mode[8];                 /* 100 */
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[8];               /* 148 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157 */
  char magic[6];                /* 257 */
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[155];             /* 345 */
                                /* 500 */

public:
    size_t file_size()
    {
        size_t size = 0;
        size_t count = 1;

        for(uint32_t j = 11; j > 0; j--, count *= 8)
        {
            size += ((this->size[j-1] - '0') * count); // parse size thats stored as a string
        }

        return size;
    }
};

#define TAR_BLOCK_SIZE 512

ResultOr<TarBlock*> TarBlock::read(void* tarfile, size_t index)
{
    tar_rawheader* header = (tar_rawheader*)tarfile;

    for(size_t offset = 0; offset < index; offset++) {
        if(header->name[0] == '\0')
            return Result::END_OF_STREAM;

        uint32_t size = header->file_size();

        header = (tar_rawheader*)((uintptr_t)header + ((size / TAR_BLOCK_SIZE) + 1) * TAR_BLOCK_SIZE);

        if(size % TAR_BLOCK_SIZE)
            header = (tar_rawheader*)((uintptr_t)header + TAR_BLOCK_SIZE);
    }

    if(header->name[0] == '\0')
        return Result::END_OF_STREAM;

    return new TarBlock(header->name, (uintptr_t)header + 512, header->file_size());
}

TarBlock::TarBlock(const char* name, uintptr_t data, size_t size)
{
    strncpy(this->name, name, 100);
    this->data = (char*)data;
    this->size = size;
}
