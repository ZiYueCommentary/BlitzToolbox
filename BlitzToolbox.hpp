/*
* Header file of BlitzToolbox.
*/

#pragma once
#include <string>

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall
typedef const char* BBStr;

BBStr getCharPtr(std::string str) {
    char* cha = new char[str.size() + 1];
    memcpy(cha, str.c_str(), str.size() + 1);
    const char* p = cha;
    return p;
}

namespace BlitzToolBox {
    inline std::string replace_all(const std::string& string, const std::string& pattern, const std::string& newpat) {
        std::string str = string;
        const uint32_t nsize = newpat.size();
        const uint32_t psize = pattern.size();

        for (uint32_t pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize))
        {
            str.replace(pos, psize, newpat);
        }
        return str;
    }

    inline std::string json_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "\\", "\\\\");
        result = replace_all(result, "\"", "\\\"");
        return result;
    }

    inline std::string html_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "<", "&lt;");
        result = replace_all(result, ">", "&gt;");
        return result;
    }

    inline std::string xml_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "\"", "&quot;");
        result = replace_all(result, "&", "&amp;");
        result = replace_all(result, "'", "&apos;");
        result = replace_all(result, ">", "&gt;");
        result = replace_all(result, "<", "&lt;");
        return result;
    }
}