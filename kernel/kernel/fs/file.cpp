#include "fs.h"
#include "path.h"

#include <string.h>
#include <stdlib.h>

namespace fs {

file* alloc_file(const char* name)
{
    file* f = (file*)malloc(sizeof(file));

    strncpy(f->name, name, PATH_FILE_NAME_SIZE);

    return f;
}

Result file_create(directory* relative, const char* path, int flags)
{
    __unused(flags); // TODO: add flag support

    char* dir_path = new char[strlen(path)];
    char file_name[PATH_FILE_NAME_SIZE];
    file* f = nullptr;

    path_split(path, dir_path, strlen(path), file_name, PATH_FILE_NAME_SIZE);

    auto parentE = fs::get_directory(relative, dir_path);
    if(!parentE.succeed()) return Result::OUT_OF_MEMORY;
    f = alloc_file(file_name);
    f->parent = parentE.value();
    parentE.value()->files.push(f);

    delete[] dir_path;
    return f == nullptr ? Result::NO_SUCH_FILE : Result::SUCCEED;
}

} 
