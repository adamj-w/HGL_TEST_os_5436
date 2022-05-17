#pragma once

#include <libsystem/Types.h>
#include <libutil/LinkedList.h>
#include <libsystem/ErrorOr.h>

#define PATH_SIZE 1024
#define PATH_FILE_NAME_SIZE 128

// TODO: use virtual instead of handlers
namespace fs {

struct file;
struct directory;

typedef Result (*file_open_t)(struct file* file);
typedef void (*file_close_t)(struct file* file);
typedef void (*file_write_t)(struct file* file, uint32_t off, const void* buffer, size_t n);
typedef void (*file_read_t)(struct file* file, uint32_t off, void* buffer, size_t n);

struct dentry {
    char name[PATH_SIZE];
    bool is_dir;
};

struct directory {
    char name[PATH_FILE_NAME_SIZE];

    LinkedList<file*> files;
    LinkedList<directory*> directories;

    struct directory* parent;
};

struct file_ops {
    file_open_t open;
    file_close_t close;
    file_read_t read;
    file_write_t write;
};

struct file {
    dev_t device;
    uint32_t inode; // TODO: upgrade inode

    char name[PATH_FILE_NAME_SIZE];
    struct directory* parent;

    struct file_ops ops;

    uint32_t data;
};

void initialize_filesystem();

directory* root_dir();

ResultOr<directory*> get_directory(directory* relative, const char* path);
ResultOr<file*> get_file(directory* relative, const char* path);

Result directory_create(directory* relative, const char* path, int flags);
// TODO: remove ops param and set to find last mounted directories file_ops
Result file_create(directory* relative, const char* path, void* data, int flags, file_ops& ops);

}
