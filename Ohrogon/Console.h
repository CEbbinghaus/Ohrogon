#include <stdio.h>
#include "String.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
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

            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(hOut, dwMode))
            {
                return 1;
            }
        #endif
        return 0;
    }
  public:
    
    static void Log(const String& message, const char* file, const char* line){
        printf("Log: %s", message);
    }

    static void Error(const String& message){
        String formatter = String::format("%sError:%s %s\n", RED, Nothing, "%s");
        printf(formatter, message);
    }
    static void Error(const String& message){
        String formatter = String::format("%Warning:%s %s\n", YELLOW, Nothing, "%s");
        printf(formatter, message);
    }
};

int Console::statusCode = Console::init();

#undef RED
#undef BLUE
#undef CYAN
#undef GREEN
#undef YELLOW
#undef Nothing