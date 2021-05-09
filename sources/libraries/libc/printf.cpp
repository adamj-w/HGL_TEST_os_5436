#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

#include <libsystem/Stream.h>
#include <libruntime/RefPtr.h>

int write(hegel::RefPtr<hegel::Stream>* stream, const char* str, size_t n)
{
    auto error = (*stream)->write((void*)str, n);
    if(error.error() != hegel::Error::SUCCEED) return -1;
    return error.value();
}

int write_to_buf(const char* str, size_t n, void* data) {
    memcpy(data, (const void*)str, n);
    return n;
}

int write_to_FILE(const char* str, size_t n, void* file) {
    return write(reinterpret_cast<hegel::RefPtr<hegel::Stream>*>(file), str, n);
}

inline void internal_fprintf_write(int (*write)(const char*, size_t, void*), size_t& maxlen, int& ret, void* data, const char* str, size_t n)
{
    if(n > maxlen) n = maxlen;
    int tmp = (*write)(str, n, data);
    maxlen -= n;
    if(ret == -1 || tmp == -1) ret = -1;
    else ret += tmp;
}

/*inline void internal_fprintf_s_to_write(int (*write)(const char*, size_t, void*), size_t& maxlen, int& ret, void* data, 
    int num, u8 base, size_t len, bool plus = false, bool uppercase = false) 
{
    if(num < 0) {
        internal_fprintf_write(write, maxlen, ret, data, "-", 1);
        num *= -1;
    } else if(plus) {
        internal_fprintf_write(write, maxlen, ret, data, "+", 1);
    }

    char digit;
    for(s32 i = len-1; i >= 0; i--) {
        unsigned int place = ullpow(base, i);
        unsigned int val = ((num / place) % base);
        if(base <= 10) {
            digit = '0' + val;
        } else {
            if(val >= 10)
            {
                digit = (uppercase ? 'A' : 'a') + val % 10;
            } else {
                digit = '0' + val;
            }
        }
        internal_fprintf_write(write, maxlen, ret, data, &digit, 1);
    }
}*/

inline void internal_fprintf_u_to_write(int (*write)(const char*, size_t, void*), size_t& maxlen, int& ret, void* data, 
    unsigned int num, uint8_t base, size_t len, bool uppercase = false) 
{
    char digit;
    for(int32_t i = len-1; i >= 0; i--) {
        unsigned int place = ullpow(base, i);
        unsigned int val = ((num / place) % base);
        if(base <= 10) {
            digit = '0' + val;
        } else {
            if(val >= 10)
            {
                digit = (uppercase ? 'A' : 'a') + val % 10;
            } else {
                digit = '0' + val;
            }
        }
        internal_fprintf_write(write, maxlen, ret, data, &digit, 1);
    }
}

inline size_t numlen(int n, uint8_t base) {
    size_t s = 0;
    if (n < 0)
    {
        n *= -1;
    }
    
    do {
        n /= base;
        s++;
    } while(n > 0);
    
    return s;  
}

inline size_t unumlen(unsigned int n, uint8_t base) {
    size_t s = 0;
    
    do {
        n /= base;
        s++;
    } while(n > 0);
    
    return s;  
}

#define FLAGS_LEFT (1<<0)
#define FLAGS_PLUS (1<<1)
#define FLAGS_SPACE (1<<2)
#define FLAGS_HASH (1<<3)
#define FLAGS_ZEROPAD (1<<4)

int internal_fprintf(int (*write)(const char*, size_t, void*), 
    void* func_data, size_t maxlen, const char* fmt, va_list args)
{

    int ret = 0;
    size_t i = 0;
    size_t prev = 0;
    size_t total = maxlen;
    while(fmt[i] != '\0')
    {
        if(fmt[i] == '%') {
            internal_fprintf_write(write, total, ret, func_data, fmt+prev, i-prev);
            char next = fmt[++i];

            prev = i;

            uint8_t flags = 0;

            do {
            switch (next)
            {
            case '-': flags |= FLAGS_LEFT; break;
            case '+': flags |= FLAGS_PLUS; break;
            case ' ': flags |= FLAGS_SPACE; break;
            case '#': flags |= FLAGS_HASH; break;
            case '0': flags |= FLAGS_ZEROPAD; break;
            case '%':
                goto parse_out;
            default:
                goto flag_parse_out;
            }

            next = fmt[++i];

            } while (true); 
flag_parse_out:

            char* after;
            uint32_t padding = strtoul(fmt+i, &after, 10);
            i = after-fmt;
            next = fmt[i];

            // TODO: finish writing the numbers and format things

            bool uppercase = false;
            size_t len;
            switch(next) {
            case 's': {
                const char* str = va_arg(args, char*);
                len = strlen(str);
                if(flags & FLAGS_LEFT) {
                    internal_fprintf_write(write, total, ret, func_data, str, len);
                    if(len < padding) {
                        for(uint32_t j = len; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, " ", 1);
                        }
                    }
                } else {
                    if(len < padding) {
                        for(uint32_t j = len; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, " ", 1);
                        }
                    }
                    internal_fprintf_write(write, total, ret, func_data, str, len);
                }
            } break;
            case 'i': {
                unsigned int num = va_arg(args, int);
                len = unumlen(num, 10);
                size_t padlen = len;
                if(flags & FLAGS_LEFT) {
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                    if(len < padding) {
                        for(uint32_t j = padlen; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, (flags & FLAGS_ZEROPAD) ? "0" : " ", 1);
                        }
                    }
                } else {
                    if(len < padding) {
                        if(flags & FLAGS_ZEROPAD) {
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, "0", 1);
                            }
                        } else {
                            if(num < 0 || flags & FLAGS_PLUS) padlen++;
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, " ", 1);
                            }
                        }
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                }
            } break;
            case 'd': {
                int num = va_arg(args, int);
                len = numlen(num, 10);
                size_t padlen = len;
                if(flags & FLAGS_LEFT) {
                    if(num < 0 || flags & FLAGS_PLUS) { 
                        padlen++;
                        internal_fprintf_write(write, total, ret, func_data, num < 0 ? "-" : "+", 1);
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                    if(len < padding) {
                        for(uint32_t j = padlen; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, (flags & FLAGS_ZEROPAD) ? "0" : " ", 1);
                        }
                    }
                } else {
                    if(len < padding) {
                        if(flags & FLAGS_ZEROPAD) {
                            if(num < 0 || flags & FLAGS_PLUS) { 
                                padlen++;
                                internal_fprintf_write(write, total, ret, func_data, num < 0 ? "-" : "+", 1);
                            }
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, "0", 1);
                            }
                        } else {
                            if(num < 0 || flags & FLAGS_PLUS) padlen++;
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, " ", 1);
                            }
                            if(num < 0 || flags & FLAGS_PLUS) {
                                internal_fprintf_write(write, total, ret, func_data, num < 0 ? "-" : "+", 1);
                            }
                        }
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                }
            } break;
            case 'u': {
                unsigned int num = va_arg(args, unsigned int);
                len = numlen(num, 10);
                size_t padlen = len;
                if(flags & FLAGS_LEFT) {
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                    if(len < padding) {
                        for(uint32_t j = padlen; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, (flags & FLAGS_ZEROPAD) ? "0" : " ", 1);
                        }
                    }
                } else {
                    if(len < padding) {
                        for(uint32_t j = padlen; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, (flags & FLAGS_ZEROPAD) ? "0" : " ", 1);
                        }
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 10, len);
                }
            } break;
            case 'X':
                uppercase = true;
                __attribute__ ((fallthrough));
            case 'x': {
                unsigned int num = va_arg(args, unsigned int);
                len = numlen(num, 16);
                size_t padlen = len;
                if(flags & FLAGS_LEFT) {
                    if(flags & FLAGS_HASH) {
                        padlen += 2;
                        internal_fprintf_write(write, total, ret, func_data, "0x", 2);
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 16, len, uppercase);
                    if(len < padding) {
                        for(uint32_t j = padlen; j < padding; j++) {
                            internal_fprintf_write(write, total, ret, func_data, " ", 1);
                        }
                    }
                } else {
                    if(len < padding) {
                        if(flags & FLAGS_ZEROPAD) {
                            if(flags & FLAGS_HASH) {
                                padlen += 2;
                                internal_fprintf_write(write, total, ret, func_data, "0x", 2);
                            }
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, "0", 1);
                            }
                        } else {
                            if(flags & FLAGS_HASH) {
                                padlen += 2;
                            }
                            for(uint32_t j = padlen; j < padding; j++) {
                                internal_fprintf_write(write, total, ret, func_data, " ", 1);
                            }
                            if(flags & FLAGS_HASH) {
                                internal_fprintf_write(write, total, ret, func_data, "0x", 2);
                            }
                        }
                    } else {
                        if(flags & FLAGS_HASH) {
                            internal_fprintf_write(write, total, ret, func_data, "0x", 2);
                        }
                    }
                    internal_fprintf_u_to_write(write, total, ret, func_data, num, 16, len, uppercase);
                }
            } break;
            default:
                goto parse_out;
                break;
            }

            prev = ++i;
        } else {
parse_out:

        i++;
        }
    }

    internal_fprintf_write(write, total, ret, func_data, fmt+prev, i-prev);

    return ret;
}

int vsnprintf(char* buf, size_t size, const char* fmt, va_list args)
{
    return internal_fprintf(write_to_buf, (void*)buf, size, fmt, args);
}

int vfnprintf(FILE* stream, size_t size, const char* fmt, va_list args)
{
    return internal_fprintf(write_to_FILE, (void*)stream, size, fmt, args);
}

int vfprintf(FILE* stream, const char* fmt, va_list args)
{
    return vfnprintf(stream, (size_t)-1, fmt, args);
}

int printf(const char* fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    int ret = vfnprintf(stdout, (size_t)-1, fmt, args);
    va_end(args);
    return ret;
}

int vprintf(const char* fmt, va_list args)
{
    return vfnprintf(stdout, (size_t)-1, fmt, args);
}

int fprintf(FILE* stream, const char* fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    int ret = vfnprintf(stream, (size_t)-1, fmt, args);
    va_end(args);
    return ret;
}

int sprintf(char* buf, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = vsprintf(buf, fmt, args);
    va_end(args);
    return ret;
}

int vsprintf(char* buf, const char* fmt, va_list args)
{
    return vsnprintf(buf, (size_t)-1, fmt, args);
}

int snprintf(char* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buf, size, fmt, args);
    va_end(args);
    return ret;
}
