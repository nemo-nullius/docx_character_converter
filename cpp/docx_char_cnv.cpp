#include "Config.h"
#include "common.h"
#include "XMLSimpParser.h"
#include "DocxXML.h"
#include "Utils.h"
#include "SimpZip.h"

int main(int argc, char * argv[]){
    if (argc < 3) {
        std::cout << config::errCmdStr << std::endl;
        return -1;
    }
    const string docxFilename = argv[1];
    string openccConfigFilename = argv[2];
    string exeFilename, exeFolderpath;
    int splitFilepathFlg = Utils::splitFilepath(string(argv[0]), exeFolderpath, exeFilename);

    int configFileFlg = Utils::openccConfigFileLocator(exeFolderpath, openccConfigFilename);
    std::cout << "[INFO] Using opencc config file at " + openccConfigFilename << std::endl;
    if (configFileFlg!=0) {
        std::cout << "[ERR] Could not find opencc config file!" << std::endl;
        return -1;
    }
    const string docxFilenameNew = docxFilename.substr(0, docxFilename.length()-5) + "_modified.docx";
    const string htmlFilenameNew = docxFilename.substr(0, docxFilename.length()-5) + "_forcheck.html";
    Utils::copyFile(docxFilename.c_str(), docxFilenameNew.c_str());
    SimpZip docxZip(docxFilenameNew.c_str());
    string htmlContent;
    for (const char * docxSubFile: config::docxSubFileCnvList) {
        string originalXML;
        //Utils::zipReadOneFile(docxFilename.c_str(), docxSubFile, originalXML);
        docxZip.zipReadOneFile(docxSubFile, originalXML);
        //std::cout << originalXML <<std::endl;
        auto xmlparser = XMLSimpParser(originalXML);
        auto docxobj = DocxXML(xmlparser, openccConfigFilename.c_str());
        size_t originalZiSum = docxobj.getZiSum();
        string convertedXML, convertedHTML;
        size_t convertedZiSum = docxobj.convert(convertedXML, convertedHTML);
        //size_t convertedZiSum2 = docxobj.getZiSum();
        std::cout << docxSubFile << "\tOriginal: " << originalZiSum << "\tConverted: " << convertedZiSum << std::endl;
        //std::cout << convertedXML << std::endl;
        docxZip.zipUpdateOneFile(docxSubFile, convertedXML);
        //Utils::zipUpdateOneFile(docxFilenameNew.c_str(), docxSubFile, convertedXML);
        htmlContent += "<div class=\""+string(docxSubFile)+"\">\n"+convertedHTML + "</div>\n<br />\n";
    }
    htmlContent = "<!DOCTYPE html>\n<html>\n<body>\n"+htmlContent+"</body>\n</html>";
    Utils::simpWriteFile(htmlFilenameNew.c_str(), std::ios::binary, htmlContent);
    std::cout << "Done." <<std::endl;
    return 0;
};
