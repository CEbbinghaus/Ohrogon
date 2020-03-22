#pragma once

#include <stdio.h>
#include "String.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define RED "\033[1;31m"
#define BLUE "\033[38;5;39m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define ORANGE "\033[38;5;166m"
#define TESTING "\033[0;2;127;238;255m"
#define Nothing "\033[0m"



class Console{
    static int statusCode;
    static int init(){
        #ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE)
            {
                return 1;
            }

            DWORD dwMode = 0;
            if (!GetConsoleMode(hOut, &dwMode))
            {
                return 1;
            }
            #ifdef ENABLE_VIRTUAL_TERMINAL_PROCESSING
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                if (!SetConsoleMode(hOut, dwMode))
                {
                    return 1;
                }
            #endif
        #endif
        return 0;
    }
  public:
    
    static void Log(const String& message){
        printf((String::format("%sLog:%s ", BLUE, Nothing) + message) + "\n");
    }

    static void Warning(const String& message){
        printf((String::format("%sWarning:%s ", ORANGE, Nothing) + message) + "\n");
    }

    static void Error(const String& message){
        printf((String::format("%sError:%s ", RED, Nothing) + message) + "\n");
    }
};

int Console::statusCode = Console::init();

#undef RED
#undef BLUE
#undef CYAN
#undef GREEN
#undef ORANGE
#undef YELLOW
#undef TESTING
#undef Nothing