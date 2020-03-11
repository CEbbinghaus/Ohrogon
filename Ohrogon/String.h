#pragma once;
#include <stdarg.h>

class String {
    const char* data;

    static int findMin(int x, int y, int z) {
        if (x <= y && x <= z)
            return x;
        else if (y <= x && y <= z)
            return y;
        else
            return z;
    }

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

    static int LevinSteinDistance(const String& a, const String& b) {
        // Declaring a 2D array on the heap dynamically:
        int len_a = strlen(a.data) + 1;
        int len_b = strlen(b.data) + 1;

        int** d = new int*[len_a];
        for (int i = 0; i < len_a; i++)
            d[i] = new int[len_b];

        // Initialising first column:
        for (int i = 0; i < len_a; i++)
            d[i][0] = i;

        // Initialising first row:
        for (int j = 0; j < len_b; j++)
            d[0][j] = j;

        // Applying the algorithm:
        int insertion, deletion, replacement;

        for (int i = 1; i < len_a; i++) {
            for (int j = 1; j < len_b; j++) {
                if (a.data[i - 1] == b.data[j - 1]) {
                    d[i][j] = d[i - 1][j - 1];
                } else {
                    // Choosing the best option:
                    insertion = d[i][j - 1];
                    deletion = d[i - 1][j];
                    replacement = d[i - 1][j - 1];

                    d[i][j] = 1 + findMin(insertion, deletion, replacement);
                }
            }
        }

        int answer = d[len_a][len_b];

        for (int i = 0; i < len_a + 1; i++) {
            delete[] d[i];
        }
        delete[] d;

        return answer;
    }

    String(const char* data) : data(data) {}

    ~String() {
        free((void*)data);
    }

    bool operator ==(const String& other){
        return strcmp(this->data, other.data) == 0;
    }
    
    bool operator !=(const String& other){
        return strcmp(this->data, other.data) != 0;
    }

    operator const char*() {
        return data;
    }
};