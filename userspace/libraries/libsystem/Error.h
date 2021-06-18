#pragma once

#define ERROR_LIST(__ITEM) \
    __ITEM(SUCCEED) \
    __ITEM(NOT_IMPLEMENTED) \
    __ITEM(READ_ONLY) \
    __ITEM(WRITE_ONLY) \
    __ITEM(END_OF_STREAM) \
    __ITEM(NO_SUCH_THREAD) \
    __ITEM(NO_SUCH_FILE) \
    __ITEM(NO_CHILD_THREAD) \
    __ITEM(NO_SUCH_PROCESS) \
    __ITEM(NO_CHILD_PROCESS) \
    __ITEM(OUT_OF_MEMORY) \
    __ITEM(ALREADY_PRESENT)

enum class Result
{
#define ERROR_ENUM_ENTRY(__entry) __entry,
    ERROR_LIST(ERROR_ENUM_ENTRY)
};
