#ifndef DOCX_CHAR_CNV_UTILS_H
#define DOCX_CHAR_CNV_UTILS_H
#include <fstream>
#include "common.h"
#include <zip.h>

namespace Utils{
    void copyFile(const char * srcFilepath, const char * dstFilepath);
    void simpWriteFile(const char * filename, std::ios_base::openmode mode, const string & content);

    void zipUpdateOneFile(const char *zipFilename, const char *inZipFilename, const string &content);

    void zipReadOneFile(const char *zipFilename, const char *inZipFilename, string &content);

    int openccConfigFileLocator(const string &exeCallPath, string &configFilePath);

    int splitFilepath(const string &str, string &folderpath, string &filename);
}
#endif //DOCX_CHAR_CNV_UTILS_H
