cmake_minimum_required(VERSION 3.14)

project(docx_char_cnv)

option(VCPKG_TARGET_TRIPLET "x64-windows")
set(CMAKE_CXX_STANDARD 17)

set(OPENCC_HEAD D:/codemine/OpenCC/src/)
include_directories(${OPENCC_HEAD})
set(OPENCC_LIB D:/codemine/OpenCC/cmake-build-release/src/opencc.lib)

set(ZIPLIB_HEAD D:/codeex/libzip-1.7.1/libzip-1.7.1/lib/)
set(ZIPCONF_HEAD D:/codeex/libzip-1.7.1/libzip-1.7.1/cmake-build-release/)
#set(ZIPLIB_LIB D:/codeex/libzip-1.7.1/libzip-1.7.1/cmake-build-release/zip.lib)
set(ZIPLIB_LIB D:/codeex/vcpkg/installed/x64-windows/lib/zip.lib)
include_directories(${ZIPLIB_HEAD})
include_directories(${ZIPCONF_HEAD})

add_executable(docx_char_cnv XMLSimpParser.h common.h docx_char_cnv.cpp DocxXML.h Utf8Obj.h ${OPENCC_HEAD}/opencc.h SimpZip.h Config.h docx_char_cnv.h Utils.cpp Utils.h)
add_executable(docx_char_cnv_test test.cpp XMLSimpParser.h common.h debug.h DocxXML.h Utf8Obj.h ${OPENCC_HEAD}/opencc.h SimpZip.h Config.h docx_char_cnv.h Utils.cpp Utils.h)

target_link_libraries(docx_char_cnv ${OPENCC_LIB} ${ZIPLIB_LIB})
target_link_libraries(docx_char_cnv_test ${OPENCC_LIB} ${ZIPLIB_LIB})
