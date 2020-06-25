#ifndef DOCX_CHAR_CNV_DOCXXML_H
#define DOCX_CHAR_CNV_DOCXXML_H

#include "common.h"
#include "XMLSimpParser.h"
#include "Utf8Obj.h"
//#include "debug.h"
#include <opencc.h>

using std::string;
using std::vector;


struct DocxXML {
public:
    DocxXML(XMLSimpParser &xmlParser, const char *configFile)
            : XMLParser(xmlParser), openccConf(configFile) {
        xmlParser.parse();
    };

    size_t convert(string &convertedText, string &convertedTextHtml) {
        size_t convertedZiSum = 0;
        getTextIndexGroups();
        convertedZiSum += convertAllParas(convertedTextHtml);
        convertedText = XMLParser.counterParse();
        return convertedZiSum;
    };

    size_t getZiSum(){
        size_t ziSum = 0;
        // duplicate with DocxXML::getTextIndexGroups()
        bool isWp = false;
        bool isWt = false;
        for (const auto & stru: XMLParser.tokenizedTextList) {
            if (stru.token == XMLSimpToken::tag_wp_beg) {
                isWp = true;
            } else if (stru.token == XMLSimpToken::tag_wp_end) {
                isWp = false;
            } else if (stru.token == XMLSimpToken::tag_wt_beg) {
                isWt = true;
            } else if (stru.token == XMLSimpToken::tag_wt_end) {
                isWt = false;
            } else if (stru.token == XMLSimpToken::text && isWp && isWt) {
                ziSum += Utf8Obj::ziSum(stru.text.c_str());
            }
        }
        return ziSum;
    };

private:
    XMLSimpParser &XMLParser;
    // { {a, b, c}, {m, n} } a/b/c refers to three indices to which text of one passage is stored separately
    vector<vector<size_t>> textIndexGroups;

    const char *openccConf;

    void getTextIndexGroups(); //1

    size_t convertOnePara(vector<size_t> &, string &);

    size_t convertAllParas(string &); //2

    /* return ziSum of converted text */
    size_t splitConvertedResult(const string &, string &, string &);

};

void DocxXML::getTextIndexGroups() {
    vector<size_t> oneGroup; // indices to text of one paragraph
    bool isWp = false;
    bool isWt = false;
    for (size_t i = 0; i < XMLParser.tokenizedTextList.size(); i++) {
        if (XMLParser.tokenizedTextList[i].token == XMLSimpToken::tag_wp_beg) {
            isWp = true;
        } else if (XMLParser.tokenizedTextList[i].token == XMLSimpToken::tag_wp_end) {
            if (!oneGroup.empty()) { // empty: with wp but without wt
                textIndexGroups.push_back(oneGroup);
            }
            oneGroup.clear();
            isWp = false;
        } else if (XMLParser.tokenizedTextList[i].token == XMLSimpToken::tag_wt_beg) {
            isWt = true;
        } else if (XMLParser.tokenizedTextList[i].token == XMLSimpToken::tag_wt_end) {
            isWt = false;
        } else if (XMLParser.tokenizedTextList[i].token == XMLSimpToken::text && isWp && isWt) {
            oneGroup.push_back(i);
        }
    }
    //debug::printVecVecSize(textIndexGroups);
}

size_t DocxXML::convertOnePara(vector<size_t> &vec, string &oneParaConvertedHtml) {
    string onePara;
    size_t convertedOneParaZiSum = 0;
    for (size_t index: vec) {
        onePara += XMLParser.tokenizedTextList[index].text;
    }
    //std::cout << onePara << std::endl;
    // convert by opencc
    opencc_t confFile = opencc_open(openccConf);
    char * transRes = opencc_convert_utf8(confFile, onePara.c_str(), onePara.length());
    string oneParaConvertedWithNumber = string(transRes);
    opencc_convert_utf8_free(transRes);
    opencc_close(confFile);
    // process opencc result
    string oneParaConverted;
    convertedOneParaZiSum += splitConvertedResult(oneParaConvertedWithNumber, oneParaConverted, oneParaConvertedHtml);
    // to fill the converted text in the original XML file
    size_t offset = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        size_t index = vec[i];
        size_t originalTextLen = XMLParser.tokenizedTextList[index].text.length();
        if (i == vec.size() - 1) { // the last Wt
            XMLParser.tokenizedTextList[index].text = oneParaConverted.substr(offset);
        } else {
            XMLParser.tokenizedTextList[index].text = oneParaConverted.substr(offset, originalTextLen);
        }
        offset += originalTextLen;
    }
    return convertedOneParaZiSum;
}

size_t DocxXML::convertAllParas(string &htmlAllParas) {
    size_t convertedZiSumAllParas = 0;
    for (auto &vec : textIndexGroups) {
        string oneParaHtml;
        convertedZiSumAllParas += convertOnePara(vec, oneParaHtml);
        htmlAllParas += "<p>" + oneParaHtml + "</p>\n";
    }
    //htmlAllParas = "<div>\n" + htmlAllParas + "</div>\n";
    return convertedZiSumAllParas;
}

size_t DocxXML::splitConvertedResult(const string &converted, string &convertedText, string &convertedHtml) {
    //converted result: (.\d)*
    string convertedZi;
    size_t ziSumWithNumber = 0;
    size_t index = 0;
    while (index < converted.length()) {
        ziSumWithNumber += 1;
        size_t ziLen = Utf8Obj::ziLen(&converted[index]);
        if (ziSumWithNumber & 1) { // odd
            convertedZi = converted.substr(index, ziLen);
            convertedText += convertedZi;
        } else { // even
            if (ziLen != 1) {
                throw "[Err] Bad choice numbers!";
            }
            int choiceNum = converted[index] - '0';
            if (choiceNum > 1) {
                convertedHtml += "<span style=\"color: #ff0000\">" + convertedZi + "◣</span>";
            } else {
                convertedHtml += convertedZi;
            }
        }
        index += ziLen;
    }
    return ziSumWithNumber / 2;
}

#endif //DOCX_CHAR_CNV_DOCXXML_H
