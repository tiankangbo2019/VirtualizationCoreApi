//
// Created by root on 2019/11/20.
//

#include "../include/vca_toCode.h"
#include "../include/libvbox_vboxsdk_types.h"

std::u16string vca_toCode::to_utf16(std::string str){
    return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(str);
}

std::string vca_toCode::to_utf8(basic_string<char16_t> str16) {
    return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(str16);
}

std::u32string vca_toCode::to_utf32(std::string str) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(str);
}

std::string vca_toCode::to_utf8(std::u32string str32) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(str32);
}

std::wstring vca_toCode::to_wchar_t(std::string str) {
    return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(str);
}

std::string vca_toCode::to_utf8(std::wstring wstr) {
    return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(wstr);
}

std::string vca_toCode::mapTojsonstr(map<const string, const int> &map_info) {

    Json::Value jObject;

    for (map<const string, const int>::const_iterator iter = map_info.begin(); iter != map_info.end(); ++iter) {
        jObject[iter->first] = iter->second;
    }

    return jObject.toStyledString();
}

std::string vca_toCode::mapTojsonstr(map<const char *, const int> &map_info) {

    Json::Value jObject;

    for (map<const char *, const int>::const_iterator iter = map_info.begin(); iter != map_info.end(); ++iter) {
        jObject[iter->first] = iter->second;
    }

    return jObject.toStyledString();
}

std::string vca_toCode::mapTojsonstr(map<const string, const string> &map_info) {

    Json::Value jObject;

    for (map<const string, const string>::const_iterator iter = map_info.begin(); iter != map_info.end(); ++iter) {
        jObject[iter->first] = iter->second;
    }

    return jObject.toStyledString();
}