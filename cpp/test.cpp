#include "common.h"
#include "debug.h"
#include "XMLSimpParser.h"
#include "DocxXML.h"
#include "Utils.h"
#include <fstream>
#include <zip.h>

void test_parse() {
    std::ifstream inFile;
    inFile.open(R"(D:\codemine\docx_character_converter\demo\sample_1\word\endnotes.xml)");
    std::string content((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());
    std::cout << content.substr(0, 100) << std::endl;
    auto xmlparser = XMLSimpParser(content);
    auto tokenizedList = xmlparser.parse();
    debug::printVecStruct(tokenizedList);
}

void test_convert(){
    std::ifstream inFile;
    inFile.open(R"(D:\codemine\docx_character_converter\demo\sample_1\word\document.xml)");
    std::string content((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());
    auto xmlparser = XMLSimpParser(content);
    auto docx = DocxXML(xmlparser, "D:/codemine/docx_character_converter/cpp/cmake-build-debug/share/opencc/t2s.json");
    string convertedText, convertedHtmlText;
    docx.convert(convertedText, convertedHtmlText);
    std::cout << convertedText << std::endl;
    std::cout << convertedHtmlText << std::endl;
}

void test_direct_zip(){
    int err = 0;
    std::ifstream fs, fs1;
    fs.open("my_test.txt", std::ios::binary);
    fs1.open("2.pdf", std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
    std::string content1((std::istreambuf_iterator<char>(fs1)), (std::istreambuf_iterator<char>()));

    zip* z = zip_open("foo.zip", ZIP_CREATE, &err);
    zip_source_t* zs;

    zs = zip_source_buffer(z, content.c_str(), content.length(), 0);

    zip_file_add(z, "test.txt", zs, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
    zs = zip_source_buffer(z, content1.c_str(), content1.length(), 0);
    zip_file_add(z, "test.pdf", zs, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);

    zip_close(z);
}

void test_zip_replace(){
    int err = 0;
    string content = "abc";

    zip* z = zip_open("D:/codemine/docx_character_converter/cpp/test/sample_1_modified.docx", 0, &err); // error if file does not exist
    zip_source_t* zs;

    zs = zip_source_buffer(z, content.c_str(), content.length(), 0);

    zip_file_add(z, "word/document.xml", zs, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8); // overwrite existing file

    zip_close(z);
}

void test_zip_read() {
    int err = 0;

    zip* z = zip_open("D:/codemine/docx_character_converter/cpp/test/sample_1.docx", 0, &err); // error if file does not exist
    zip_file_t * zf = zip_fopen(z, "word/document.xml", ZIP_FL_ENC_UTF_8);
    struct zip_stat zf_stat;
    zip_stat(z, "word/document.xml", ZIP_FL_ENC_UTF_8, &zf_stat);
    //char buf[zf_stat.size];
    char * buf = (char*)malloc(zf_stat.size+1);
    std::cout<<zf_stat.size << std::endl;
    std::cout<<zf_stat.comp_size << std::endl;
    zip_fread(zf, buf, zf_stat.size);
    buf[zf_stat.size] = '\0';
    string content = string(buf);
    std::cout << content.length() << std::endl;
    char tmp[4]{'a','b','c','\0'};
    string tmps = string(tmp);
    std::cout << buf[zf_stat.size-1] << std::endl;
    std::cout << buf[zf_stat.size-2] << std::endl;
    std::cout << tmps.length() << std::endl;
    Utils::simpWriteFile("../test/doc.xml", std::ios::binary, buf);
    free(buf);
    zip_fclose(zf);
    zip_close(z);
}

void test_ziSum(){
    string input = u8"好学深思ab123𣊫";
    //std::cin >> input;
    //std::cout << Utf8Obj::ziSum(input.c_str()) << std::endl;
    std::cout << Utf8Obj::ziSum(input.c_str()) << std::endl;
}

int main() {
    test_parse();
    //test_convert();
    //test_direct_zip();
    //test_zip_replace();
    //test_zip_read();
    //test_ziSum();
}

