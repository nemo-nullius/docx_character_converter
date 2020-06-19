"""
Overall thought:
Find <w:p ...>...</w:p> as a isolated group for conversion.
Extract <w:t ...>(...)</w:t> in that group and put them together as a new string.
Convert the new string.
Put the chars in the new string back to their own <w:t ...>(...)</w:t>
Find next isolated group.
"""
import os
import re
import sys
import regex
import opencc_mod
import zipfile
import tempfile
import ProcessFile as PF


def generate_zip(zipname, filename_data_dict: dict, update=False):
    # generate a temp file
    tmpfd, tmpname = tempfile.mkstemp(dir=os.path.dirname(zipname))
    os.close(tmpfd)

    # create a temp copy of the archive without filename
    with zipfile.ZipFile(zipname, 'r') as zin:
        with zipfile.ZipFile(tmpname, 'w') as zout:
            zout.comment = zin.comment  # preserve the comment
            for item in zin.infolist():
                if item.filename not in filename_data_dict.keys():
                    zout.writestr(item, zin.read(item.filename))

    # replace with the temp archive
    if update:
        os.remove(zipname)
        os.rename(tmpname, zipname)
    else:
        zipname0, zipname1 = os.path.splitext(zipname)
        zipname = "".join([zipname0, "_modified", zipname1])
        os.rename(tmpname, zipname)

    # now add filename with its new data
    with zipfile.ZipFile(zipname, mode='a', compression=zipfile.ZIP_DEFLATED) as zf:
        for filename, data in filename_data_dict.items():
            zf.writestr(filename, data)


class DocxHanlder(object):
    def __init__(self, filepath, configfile="t2s.json"):
        self.filepath = filepath
        self.zip_document_path = "word/document.xml"
        self.zip_endnotes_path = "word/endnotes.xml"
        self.zip_footnotes_path = "word/footnotes.xml"
        self.configfile = configfile
        self.filename8, self.filename3 = os.path.splitext(filepath)
        with zipfile.ZipFile(filepath) as docx:
            self.s_document = docx.read(self.zip_document_path).decode("utf-8")
            self.s_endnotes = docx.read(self.zip_endnotes_path).decode("utf-8")
            self.s_footnotes = docx.read(self.zip_footnotes_path).decode("utf-8")

    def _convert(self, s_A):
        converter = opencc_mod.OpenCC(self.configfile)
        s_B = converter.convert(s_A)
        # if len(s_B) != len(s_A): raise Exception("[Err] s_A s_B not same length!")
        return s_B

    def _combine_list_alternate(self, l1, l2, gap=1) -> list:
        # if len(l1) - len(l2) != 0 and len(l1) - len(l2) != 1:
        # raise Exception("[ERR] len(l1)-len(l2) should be 0 or 1")
        result = [None] * (len(l1) + len(l2))
        i1 = 0
        i2 = 0
        for i in range(0, len(result)):
            if (i + 1) % (gap + 1) == 0:
                result[i] = l2[i2]
                i2 += 1
            else:
                result[i] = l1[i1]
                i1 += 1
        # result[::2] = l1
        # result[1::2] = l2
        return result

    def _convert_xml(self, sxml, fn_conv):
        """
        for fn_conv which outputs converted characters
        along with numbers indicating how many choices there are in the dictionary entry
        :param sxml:
        :param fn_conv:
        :param fn_conv_flg: Whether the conv function will also give marks of alternative choices
        :return:
        """
        sum_input_char = 0
        sum_output_char = 0
        html_para_list = []
        para_wrapper_list = re.split(r"<w:p.*?</w:p>", sxml)
        para_list = re.findall(r"<w:p.*?</w:p>", sxml)
        if len(para_wrapper_list) != len(para_list) + 1: raise Exception("[ERR] Wrong para_list!")
        para_list_new = []
        for para in para_list:
            p_frag = r"(?:(?<=<w:t>)|(?<=<w:t [^>]+>)).*?(?=<\/w:t>)" # <w:t xml:space="preserve"> </w:t> is included
            frag_wrapper_list = regex.split(p_frag, para)
            frag_list = regex.findall(p_frag, para)
            if len(frag_wrapper_list) != len(frag_list) + 1: raise Exception("[ERR] Wrong frag_list!")
            para_text = "".join(frag_list)
            sum_input_char += len(para_text)
            para_conv = fn_conv(para_text)
            if not (re.search(r"^(.\d)*$", para_conv)):
                raise Exception("[Err] para_conv not correct!\n" + para_conv)
            # [{char: mark}, {"祇": 2}, ...]
            para_char_mark_list = [{"c": x, "m": int(y)} for x, y in zip(para_conv[::2], para_conv[1::2])]
            para_text_new = "".join([x["c"] for x in para_char_mark_list])
            sum_output_char += len(para_text_new)
            frag_list_new = []
            frag_offset = 0
            if len(para_text) != len(para_text_new):
                print("[Warn] Input & Output not equal char numbers.")
            # If len(para_text) != len(para_text_new)
            # Then all the differences are accumulated to the final append call
            for frag in frag_list[:-1]:
                frag_len = len(frag)
                frag_list_new.append(para_text_new[frag_offset: frag_offset + frag_len])
                frag_offset += frag_len
            frag_list_new.append(para_text_new[frag_offset:])

            para_new = "".join(self._combine_list_alternate(frag_wrapper_list,
                                                            ["%s" % x for x in frag_list_new]))
            para_list_new.append(para_new)

            # make html
            html_para = ""
            for char_mark in para_char_mark_list:
                char = char_mark["c"]
                mark = char_mark["m"]
                if mark > 1:
                    html_para = '%s<span style="color: #ff0000">%s◣</span>' %(html_para, char)
                else:
                    html_para += char
            html_para = '<p>%s</p>' % html_para
            html_para_list.append(html_para)

        html_body = '<body>\n%s\n</body>' % "\n".join(html_para_list)
        sxml_new = "".join(self._combine_list_alternate(para_wrapper_list, para_list_new))
        return sxml_new, html_body, sum_input_char, sum_output_char

    def test_conv_same(self):
        s = self._convert_xml(self.s_document, lambda x: x)
        PF.write_bytes_into_file("./document2.xml", s[0].encode("utf-8"))
        s = self._convert_xml(self.s_endnotes, lambda x: x)
        PF.write_bytes_into_file("./endnotes2.xml", s[0].encode("utf-8"))
        s = self._convert_xml(self.s_footnotes, lambda x: x)
        PF.write_bytes_into_file("./footnotes2.xml", s[0].encode("utf-8"))

    def convert(self):
        bdoc, htmldoc, doc_in_char_sum, doc_out_char_sum = self._convert_xml(self.s_document, self._convert)
        bfn, htmlfn, fn_in_char_sum, fn_out_char_sum = self._convert_xml(self.s_footnotes, self._convert)
        ben, htmlen, en_in_char_sum, en_out_char_sum = self._convert_xml(self.s_endnotes, self._convert)
        html = """<!DOCTYPE html>\n<html>\n%s<br />%s<br />%s<br />\n</html>""" % (htmldoc, htmlfn, htmlen)
        generate_zip(self.filepath, {self.zip_document_path: bdoc.encode("utf-8"),
                                     self.zip_endnotes_path: ben.encode("utf-8"),
                                     self.zip_footnotes_path: bfn.encode("utf-8"), })
        PF.write_into_file(self.filename8+"_forcheck.html", html)
        print("Finish.")
        print("[Document] Input %d\tOutput %d" %(doc_in_char_sum, doc_out_char_sum))
        print("[Footnote] Input %d\tOutput %d" %(fn_in_char_sum, fn_out_char_sum))
        print("[Endnote] Input %d\tOutput %d" %(en_in_char_sum, en_out_char_sum))


def main():
    if len(sys.argv) < 3:
        print("[Err] Bad parameters.\nUsage:\npython docx_char_cnv.py <filename> <config>")
        print("For example:\npython docx_char_cnv.py sample.docx t2s.json")
        sys.exit(-1)
    filename = sys.argv[1]
    configfile = sys.argv[2]
    docx = DocxHanlder(filename, configfile=configfile)
    docx.convert()


if __name__ == "__main__":
    main()
