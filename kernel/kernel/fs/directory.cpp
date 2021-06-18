#include "fs.h"
#include "path.h"

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

Result directory_create(directory* relative, const char* path, int flags)
{
    __unused(flags); // TODO: add flags

    char* dir_path = new char[strlen(path)];
    char dir_name[PATH_FILE_NAME_SIZE];
    directory* dir = nullptr;

    path_split(path, dir_path, strlen(path), dir_name, PATH_FILE_NAME_SIZE);
       
    auto parentE = fs::get_directory(relative, dir_path);
    if(!parentE.succeed()) return Result::NO_SUCH_FILE;
    dir = alloc_dir(dir_name);
    dir->parent = parentE.value();
    parentE.value()->directories.push(dir);

    delete[] dir_path;
    return dir == nullptr ? Result::OUT_OF_MEMORY : Result::SUCCEED;
}

}
