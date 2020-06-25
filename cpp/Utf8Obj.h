#ifndef DOCX_CHAR_CNV_UTF8OBJ_H
#define DOCX_CHAR_CNV_UTF8OBJ_H

#include "common.h"

struct Utf8Obj {
public:
    static size_t ziLen(const char *str) {
        char ch = *str;
        if ((ch & 0xF0) == 0xE0) {
            return 3;
        } else if ((ch & 0x80) == 0x00) {
            return 1;
        } else if ((ch & 0xE0) == 0xC0) {
            return 2;
        } else if ((ch & 0xF8) == 0xF0) {
            return 4;
        } else if ((ch & 0xFC) == 0xF8) {
            return 5;
        } else if ((ch & 0xFE) == 0xFC) {
            return 6;
        }
        return 0;
    }

    static size_t ziSum(const char *str) {
        size_t zisum = 0;
        size_t oneLen;
        const char *p = str;
        while (*p){
            oneLen = ziLen(p);
            zisum += 1;
            p += oneLen;
        }
        return zisum;
    }

private:
};

#endif //DOCX_CHAR_CNV_UTF8OBJ_H
