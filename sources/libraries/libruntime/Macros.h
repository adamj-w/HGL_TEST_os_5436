#pragma once

#ifndef __BUILD_TARGET__
#define __BUILD_TARGET__ "unknown"
#endif

#ifndef __BUILD_GITREF__
#define __BUILD_GITREF__ "unknown/unknown"
#endif

#ifndef __BUILD_UNAME__
#define __BUILD_UNAME__ "unknown"
#endif

#define __packed __attribute__((packed))

#define __aligned(__align) __attribute__((aligned(__align)))

#define __align_down(__addr, __align) ((__addr) & ~((__align)-1))

#define __align_up(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))

#define __unused(__arg) (void)(__arg)

#define __noreturn __attribute__((noreturn));

#define __unchecked_all __attribute__((no_sanitize("address", "thread", "memory", "dataflow", "cfi", "safe-stack", "undefined")))

#define __str(s) #s

#define __noncopyable(__class_name) \
    __class_name(const __class_name &) = delete; \
    __class_name& operator=(const __class_name &) = delete;

#define __nonmovable(__class_name) \
    __class_name(const __class_name &&) = delete; \
    __class_name& operator=(const __class_name &&) = delete;
