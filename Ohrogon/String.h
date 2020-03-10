#pragma once;
#include <stdarg.h>

class String {
    const char* data;

   public:
    static String format(const char* base, ...) {
        va_list argptr;
        va_start(argptr, base);
        size_t needed = vsnprintf(NULL, 0, base, argptr) + 1;
        char* buffer = (char*)malloc(needed);
        vsprintf(buffer, base, argptr);
        va_end(argptr);
        return String(buffer);
    }

    String(const char* data) : data(data) {}

    ~String() {
        free((void*)data);
    }

    operator const char*() {
        return data;
    }
};