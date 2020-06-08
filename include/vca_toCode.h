//
// Created by root on 2019/11/20.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_TOCODE_H
#define VIRTUALIZATIONCOREAPI_VCA_TOCODE_H

#include <iostream>
#include <codecvt>
#include <locale>
#include <map>
#include <jsoncpp/json/json.h>  //apt-get install libjsoncpp-dev libjsoncpp1
#include "libvbox_vboxsdk_types.h"

using namespace std;

/*
 *中文操作系统默认ansi编码,ANSI编码表示英文字符时用一个字节，表示中文用两个或四个字节.
 * unicode是国际通用编码,每个字母或汉字都是两个字节.
 * utf-8编码是unicode编码在网络之间（主要是网页）传输时的一种“变通”和“桥梁”编码.
 * 英文字母为一个字节，中文为三个字节，处理中文很不方便，将string转为u16string则每个字母或汉字为两个字节，处理起来比较方便.
 * */

class vca_toCode {

public:
    //string -> u16string
    std::u16string to_utf16(std::string str);

    //u16string -> string
    std::string to_utf8(basic_string<char16_t> str16);

    //string -> u32string
    std::u32string to_utf32(std::string str);

    //u32string -> string
    std::string to_utf8(std::u32string str32);

    //string -> wstring
    std::wstring to_wchar_t(std::string str);

    //wstring -> string
    std::string to_utf8(std::wstring wstr);

    //map -> json
    std::string mapTojsonstr(map<const std::string, const int> &);

    //map -> json
    std::string mapTojsonstr(map<const char *, const int> &);

    //map ->json
    std::string mapTojsonstr(map<const string, const string> &);
};


#endif //VIRTUALIZATIONCOREAPI_VCA_TOCODE_H
