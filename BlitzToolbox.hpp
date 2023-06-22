/*
* Header file of BlitzToolbox.
*/

#pragma once
#include <string>
#include <vector>

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall
typedef const char* BBStr;

namespace BlitzToolbox {
    _NODISCARD inline BBStr getCharPtr(const std::string& str) {
        char* cha = new char[str.size() + 1];
        memcpy(cha, str.c_str(), str.size() + 1);
        const char* p = cha;
        return p;
    }
    
    _NODISCARD _CONSTEXPR20 std::string replace_all(const std::string& string, const std::string& pattern, const std::string& newpat) {
        std::string str = string;
        const unsigned nsize = newpat.size();
        const unsigned psize = pattern.size();

        for (unsigned pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize))
        {
            str.replace(pos, psize, newpat);
        }
        return str;
    }
    
    _NODISCARD _CONSTEXPR20 std::string json_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "\\", "\\\\");
        result = replace_all(result, "\"", "\\\"");
        return result;
    }

    _NODISCARD _CONSTEXPR20 std::string html_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "<", "&lt;");
        result = replace_all(result, ">", "&gt;");
        return result;
    }

    _NODISCARD _CONSTEXPR20 std::string xml_friendly_string(const std::string& str) {
        std::string result = str;
        result = replace_all(result, "\"", "&quot;");
        result = replace_all(result, "&", "&amp;");
        result = replace_all(result, "'", "&apos;");
        result = replace_all(result, ">", "&gt;");
        result = replace_all(result, "<", "&lt;");
        return result;
    }

    _NODISCARD _CONSTEXPR20 std::vector<std::string> split_string(const std::string& str, const std::string& split) {
        std::string string = str;
        std::vector<std::string> vector;
        int pos = string.find(split);
        while (pos != -1) {
            vector.push_back(string.substr(0, pos));
            string = string.substr(pos + split.length());
            pos = string.find(split);
        }
        vector.push_back(string);
        return vector;
    }
}