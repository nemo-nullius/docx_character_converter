import codecs
import re

def read_input_file(from_file_path):
    '''
    此函數用以讀入欲比较之文件。將文件所有內容存爲list。
    '''
    result = []
    with open(from_file_path, 'r', encoding='utf-8') as f:
        result = f.readlines() # 讀入文檔的全部內容並存爲list
    return result

def read_file(from_file_path, coding='utf-8', return_type='list', errors_type='strict'):
    '''
    This func is used to read input file
    Three new features:
    1. can set the coding
    2. decide whether return str or list
    3. As for list, there is no '\n' or '\r\n' in the end
    :param from_file_path:
    :param coding:
    :return:
    '''
    coding = coding.lower()
    with open(from_file_path, 'rb') as f:
        encoded_text = f.read()
    codingInfo = re.match(r'^(utf-(?:8|(?:(?:16|32)[lb][e])))(?: (bom))?$', coding)
    if codingInfo == None:
        print('Error with param coding!')
        return None

    codeInfo = codingInfo.group(1)
    bomInfo = codingInfo.group(2)
    #print(coding, codeInfo, bomInfo)

    if bomInfo == None: # No bom
        decoded_text = encoded_text.decode(codeInfo, errors=errors_type)
    else: # with bom
        if codeInfo == 'utf-8':
            bom = codecs.BOM_UTF8
        elif codeInfo == 'utf-16le':
            bom = codecs.BOM_UTF16_LE
        elif codeInfo == 'utf-16be':
            bom = codecs.BOM_UTF16_BE
        elif codeInfo == 'utf-32le':
            bom = codecs.BOM_UTF32_LE
        elif codeInfo == 'utf-32be':
            bom = codecs.BOM_UTF32_BE
        else:
            print('Error with bom info!')
            return None
        assert encoded_text.startswith(bom)
        encoded_text = encoded_text[len(bom):]
        decoded_text = encoded_text.decode(codeInfo, errors=errors_type)

    if return_type == 'list':
        return decoded_text.splitlines()
    elif return_type == 'str':
        return decoded_text
    else:
        print('Error with param return_type!')
        return None

def write_bytes_into_file(to_file_path, s):
    with open(to_file_path, 'wb') as f:
        f.write(s)

def write_into_file(to_file_path, s):
    with open(to_file_path, 'w', encoding='utf-8') as f:
        f.write(s)

def append_to_file(to_file_path, s):
    with open(to_file_path, 'a', encoding='utf-8') as f:
        f.write(s)

def csvintolist(l, splitter='\t'):
    '''
    this func is used to make a list (which is achieved from a csv file) into a 2-dimension file
    '''
    result = []
    for ln in l:
        ln = ln.replace('\n','') # del ***the last \n*** in each line
        result.append(ln.split(splitter))
    return result

####################
# Funcs concerning column in excel
def ABCto123(s):
    '''
    Thins func is used to change 'A' to '1', 'B' to '2' and so on
    so that we could get the number of each column in excel
    :param s:
    :return:
    '''
    s = s.upper()
    s = s[::-1]
    r = 0
    for i in range(0, len(s)):
        r += (ord(s[i])-64)*(26**i)
    return r


def numberizeDColInfo(d):
    '''
    Actually, this func is like a procedure but with a return
    d is also changed at the same time
    This func is used to change column alphabet 'A' 'B' 'C' in excel to column number 0 1 2
    in dColInfo
    :param d(before): a dict contains key to column alphabet like {'KEY1': 'A', 'KEY2': 'B'}
    :return:{'KEY1': 0, 'KEY2': 1}
    '''
    r = d
    for x in r:
        r[x] = ABCto123(r[x])-1
    return r


if __name__ == '__main__':
    l_8 = read_file('./.ProcessFile_test/test.8')
    l_8_old = read_input_file('./.ProcessFile_test/test.8')
    l_16le = read_file('./.ProcessFile_test/test.16le',coding='utf-16le')
    l_16lebom = read_file('./.ProcessFile_test/test.16lebom',coding='utf-16le bom')
    l_16be = read_file('./.ProcessFile_test/test.16be',coding='utf-16be')
    l_16bebom = read_file('./.ProcessFile_test/test.16bebom',coding='utf-16be bom')
    #l_32le = read_file('./.ProcessFile_test/test.32le',coding='utf-32le')
    l_32lebom = read_file('./.ProcessFile_test/test.32lebom',coding='utf-32le bom')
    l_32be = read_file('./.ProcessFile_test/test.32be',coding='utf-32be')
    l_32bebom = read_file('./.ProcessFile_test/test.32bebom',coding='utf-32be bom')
    for i in range(0, len(l_8_old)):
        l_8_old[i] = l_8_old[i].replace('\n','')
    print(l_8_old)
    print(l_8[:3])
    print(l_16lebom[:3])
    print(l_32lebom[:3])
    assert l_8 == l_8_old
    assert l_8 == l_16le
    assert l_8 == l_16lebom
    assert l_8 == l_16be
    assert l_8 == l_16bebom
    #assert l_8 == l_32le
    assert l_8 == l_32lebom
    assert l_8 == l_32be
    assert l_8 == l_32lebom



    s_8 = read_file('./.ProcessFile_test/test.8', return_type='str')
    s_16le = read_file('./.ProcessFile_test/test.16le',coding='utf-16Le', return_type='str')
    s_16lebom = read_file('./.ProcessFile_test/test.16lebom',coding='utf-16le bom', return_type='str')
    s_16be = read_file('./.ProcessFile_test/test.16be',coding='utf-16be', return_type='str')
    s_16bebom = read_file('./.ProcessFile_test/test.16bebom',coding='utf-16be bom', return_type='str')
    #s_32le = read_file('./.ProcessFile_test/test.32le',coding='utf-32le', return_type='str')
    s_32lebom = read_file('./.ProcessFile_test/test.32lebom',coding='utf-32le bom', return_type='str')
    s_32be = read_file('./.ProcessFile_test/test.32be',coding='utf-32be', return_type='str')
    s_32bebom = read_file('./.ProcessFile_test/test.32bebom',coding='utf-32be bom', return_type='str')
    assert s_8 == s_16le
    assert s_8 == s_16lebom
    assert s_8 == s_16be
    assert s_8 == s_16bebom
    #assert s_8 == s_32le
    assert s_8 == s_32lebom
    #assert s_8 == s_32be
    assert s_8 == s_32lebom
