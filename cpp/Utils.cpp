#include "Utils.h"
#include <filesystem>

void Utils::copyFile(const char *srcFilepath, const char *dstFilepath) {
    std::ifstream src(srcFilepath, std::ios::binary);
    std::ofstream dst(dstFilepath, std::ios::binary);
    dst << src.rdbuf();
}

void Utils::simpWriteFile(const char *filename, std::ios_base::openmode mode, const string &content) {
    std::ofstream dst(filename, mode);
    dst << content;
}

void Utils::zipUpdateOneFile(const char *zipFilename, const char *inZipFilename, const string &content) {
    int err = 0;
    zip *z = zip_open(zipFilename, 0, &err); // error if file does not exist
    zip_source_t *zs;
    zs = zip_source_buffer(z, content.c_str(), content.length(), 0);
    zip_file_add(z, inZipFilename, zs, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8); // overwrite existing file
    zip_close(z);
}

void Utils::zipReadOneFile(const char *zipFilename, const char *inZipFilename, string &content) {
    int err;
    zip *z0 = zip_open(zipFilename, 0, &err); // error if file does not exist
    zip_file_t *zf = zip_fopen(z0, inZipFilename, ZIP_FL_ENC_UTF_8);
    struct zip_stat zf_stat;
    zip_stat(z0, inZipFilename, ZIP_FL_ENC_UTF_8, &zf_stat);
    char *buf = (char *) malloc(zf_stat.size + 1); // final char for '\0'
    zip_fread(zf, buf, zf_stat.size);
    buf[zf_stat.size] = '\0'; //IMPORTANT
    content = string(buf);
    free(buf);
    zip_fclose(zf);
    zip_close(z0);
}

int Utils::openccConfigFileLocator(string &configFilePath) {
    if (std::filesystem::exists(configFilePath)) {
        return 0;
    }
    string newPath;
    if (configFilePath.find('\\') == string::npos && configFilePath.find('/') == string::npos) {
        if (configFilePath.length() > 5 && configFilePath.substr(configFilePath.length() - 5, 5) == string(".json")) {
            newPath = "./share/opencc/" + configFilePath;
        } else {
            newPath = "./share/opencc/" + configFilePath + ".json";
        }
    }
    configFilePath = newPath;
    if (std::filesystem::exists(configFilePath)) {
        return 0;
    }
    return -1;
}
