//
// Created by nonel on 6/24/2020.
//

#ifndef DOCX_CHAR_CNV_SIMPZIP_H
#define DOCX_CHAR_CNV_SIMPZIP_H

#include "common.h"
#include <zip.h>

struct SimpZip {
public:
    explicit SimpZip(const char *s) : zipPath(s) {};


    void zipUpdateOneFile(const char *inZipFileName, const string &content) { // overwrite existing file
        zip_t * zfile = zip_open(zipPath, 0, &err);
        zip_source_t *zs;
        zs = zip_source_buffer(zfile, content.c_str(), content.length(), 0);
        zip_file_add(zfile, inZipFileName, zs, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
        zip_close(zfile);
    };

    int zipReadOneFile(const char *iZipFileName, string &content) {
        zip_t * zfile = zip_open(zipPath, 0, &err);
        // get file stats
        struct zip_stat zf_stat;
        zip_stat(zfile, iZipFileName, ZIP_FL_ENC_UTF_8, &zf_stat);
        // read file
        zip_file_t *zf = zip_fopen(zfile, iZipFileName, ZIP_FL_ENC_UTF_8);
        char *buf = (char *) malloc(zf_stat.size+1); // final char for '\0'
        int readBytes = zip_fread(zf, buf, zf_stat.size);
        buf[zf_stat.size] = '\0'; // IMPORTANT
        content = string(buf);
        free(buf);
        zip_fclose(zf);
        zip_close(zfile);
        return readBytes;
    };

private:
    const char *zipPath;
    int err = 0;
};

#endif //DOCX_CHAR_CNV_SIMPZIP_H
