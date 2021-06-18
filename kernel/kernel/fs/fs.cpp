#include "fs.h"
#include "path.h"

#include <string.h>
#include <stdlib.h>

namespace fs {

static directory* root;

extern directory* alloc_dir(const char* name);

void initialize_filesystem()
{
    root = alloc_dir("/");
}

directory* root_dir()
{
    return root;
}

ResultOr<directory*> get_directory(directory* relative, const char* path)
{
    char buffer[PATH_FILE_NAME_SIZE];
    directory* current = relative ? relative : root;

    for(size_t i = 0; path_read(path, i, buffer, PATH_FILE_NAME_SIZE); ++i) {
        if(strcmp(buffer, "..") == 0) {
            current = current->parent;
            assert(current);
        } else if(strcmp(buffer, ".") == 0) {
            //current = current; // nop
        } else {
            if (
                Iteration::CONTINUE == 
                current->directories.foreach([&](directory* d) -> Iteration {
                    if(strncmp(buffer, d->name, PATH_FILE_NAME_SIZE) == 0) {
                        current = d;
                        return Iteration::STOP;
                    }
                    return Iteration::CONTINUE;
                })) {
                // Didn't find anything
                return Result::NO_SUCH_FILE; // TODO: change to diff result
            }
        }
    }

    return current;
}

ResultOr<file*> get_file(directory* relative, const char* path)
{
    size_t pathLen = strlen(path);
    char* dirName = (char*)malloc(pathLen);
    char filename[PATH_FILE_NAME_SIZE];
    file* result = nullptr;

    path_split(path, dirName, pathLen, filename, PATH_FILE_NAME_SIZE);
    
    auto dirE = get_directory(relative, dirName);
    assert(dirE.succeed());

    dirE.value()->files.foreach([&](file* f) -> Iteration {
        if(strncmp(filename, f->name, PATH_FILE_NAME_SIZE) == 0) {
            result = f;
            return Iteration::STOP;
        }
        return Iteration::CONTINUE;
    });

    if(result == nullptr) return Result::NO_SUCH_FILE;

    delete[] dirName;
    return result;
}

}
