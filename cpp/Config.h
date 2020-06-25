#ifndef DOCX_CHAR_CNV_CONFIG_H
#define DOCX_CHAR_CNV_CONFIG_H

#include "common.h"

namespace config {
    std::vector<const char *> docxSubFileCnvList = {"word/document.xml", "word/footnotes.xml", "word/endnotes.xml"};
    string errCmdStr = "[Err] Bad parameters.\nUsage:\ndocx_char_cnv.exe <filename> <config>\n"
                        "For example:\ndocx_char_cnv.exe sample.docx t2s.json";
}

#endif //DOCX_CHAR_CNV_CONFIG_H
