#ifndef DOCX_CHAR_CNV_COMMON_H
#define DOCX_CHAR_CNV_COMMON_H

#include <string>
#include <vector>
#include <iostream>

using std::string;

enum XMLSimpToken{
    tag_prolog,
    tag_wp_beg,
    tag_wp_end,
    tag_wt_beg,
    tag_wt_end,
    tag_other_beg,
    tag_other_end,
    tag_other_beg_end,
    text,
};

struct TokenizedText{
    XMLSimpToken token;
    string text;
};

#endif //DOCX_CHAR_CNV_COMMON_H
