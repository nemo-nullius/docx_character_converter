#ifndef DOCX_CHAR_CNV_XML_SIMP_PARSER_H
#define DOCX_CHAR_CNV_XML_SIMP_PARSER_H

#include "common.h"

using std::string;
using std::vector;

struct XMLSimpParser {
    friend struct DocxXML;
public:
    explicit XMLSimpParser(const string &s) {
        xml = s.c_str();
        xmlp = xml;
    };

    vector<TokenizedText> parse() {
        simpParse();
        return tokenizedTextList;
    };

    string counterParse(){
        return simpCombine();
    };
private:
    const char *xml;
    const char *xmlp;
    string xmlBuf;
    vector<TokenizedText> tokenizedTextList;

    char eat() {
        return *(xmlp++);
    };

    void simpParse();

    void tokenize();

    string simpCombine(); //combine text from simpParse()
};

void XMLSimpParser::simpParse() {
    while (true) {
        const char c = eat();
        // in XML < > & should be escaped as &lt; &gt; &amp;
        switch (c) {
            case '<':
                tokenize();
                xmlBuf = string(1, c);
                break;
            case '>':
                xmlBuf.push_back(c);
                tokenize();
                xmlBuf = "";
                break;
            case '\0':
                tokenize();
                xmlBuf = "";
                return;
                break;
            default:
                xmlBuf.push_back(c);
        }
    }
}

void XMLSimpParser::tokenize() {
    if (xmlBuf.length() == 0) {
        return;
    }
    XMLSimpToken token;
    const string &xmlText = xmlBuf;
    if (xmlText[0] != '<') {
        token = XMLSimpToken::text;
    } else if (xmlText.length() >= 4 && xmlText.substr(0, 2) == "<?") { // <?...?>
        token = XMLSimpToken::tag_prolog;
    } else if (xmlText == "</w:p>") {
        token = XMLSimpToken::tag_wp_end;
    } else if (xmlText == "</w:t>") {
        token = XMLSimpToken::tag_wt_end;
    } else if (xmlText.length() >= 3 && *xmlText.rbegin() == '>' && *(xmlText.rbegin() + 1) == '/') { // </> <..../>
        token = XMLSimpToken::tag_other_beg_end;
    } else if (xmlText.length() >= 3 && xmlText.substr(0, 2) == "</") { // </...>
        token = XMLSimpToken::tag_other_end;
    } else if (xmlText == "<w:p>" ||
               (xmlText.length() >= 5 && xmlText.substr(0, 5) == "<w:p ")) {
        token = XMLSimpToken::tag_wp_beg;
    } else if (xmlText == "<w:t>" ||
               (xmlText.length() >= 5 && xmlText.substr(0, 5) == "<w:t ")) {
        token = XMLSimpToken::tag_wt_beg;
    } else {
        token = XMLSimpToken::tag_other_beg;
    }
    tokenizedTextList.push_back({token, xmlBuf});
}

string XMLSimpParser::simpCombine() {
    string content;
    for (const auto & stru : tokenizedTextList){
        content += stru.text;
    }
    return content;
}


#endif //DOCX_CHARACTER_CONVERTER_DOCX_CHAR_CNV_H
