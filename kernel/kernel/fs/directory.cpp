#include "fs.h"

#include <string.h>

namespace fs {

directory* alloc_dir(const char* name)
{
    directory* dir = new directory;

    dir->name[0] = '\0';
    strncpy(&dir->name[0], name, PATH_FILE_NAME_SIZE);
    dir->directories = LinkedList<directory*>();
    dir->files = LinkedList<file*>();

    return dir;
}

}
