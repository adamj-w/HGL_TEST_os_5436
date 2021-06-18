#include "path.h"
#include "fs.h"

#include <string.h>
#include <libsystem/Assert.h>

namespace fs {

int path_read(const char* path, size_t index, char* buffer, size_t n)
{
    size_t current_index = 0;
    buffer[0] = '\0';

    if(path[0] == '/') path++;

    for(size_t i = 0; path[i]; i++) {
        char c = path[i];

        if(current_index == index) {
            if(c == '/')
                break;
            else
                strnapd(buffer, c, n);
        }

        if(c == '/') {
            current_index++;
            buffer[0] = '\0';
        }
    }

    return (current_index == index && strlen(buffer)) ? 1 : 0;
}

size_t path_len(const char* path)
{
    char buffer[PATH_FILE_NAME_SIZE];
    size_t count;

    for(count = 0; path_read(path, count, buffer, PATH_FILE_NAME_SIZE); count++) {}

    return count;
}

void path_split(const char* path, char* dir, size_t dirN, char* file, size_t fileN)
{
    assert(!(path == NULL || dir == NULL || file == NULL));

    size_t pathlen = path_len(path);

    dir[0] = '\0';
    file[0] = '\0';

    char buffer[PATH_FILE_NAME_SIZE];

    for(size_t i = 0; path_read(path, i, buffer, PATH_FILE_NAME_SIZE); i++) {
        if(i < pathlen - 1) {
            strncat(dir, buffer, dirN);
            strnapd(dir, '/', dirN);
        } else {
            strncpy(file, buffer, fileN);
        }
    }
}

}
