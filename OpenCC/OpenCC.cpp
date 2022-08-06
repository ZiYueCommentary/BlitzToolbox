/*
* OpenCC extension - A part of BlitzToolbox
* Conversion between Traditional and Simplified Chinese.
* 
* v1.0 2022.7.27
*/

#include "opencc/SimpleConverter.hpp"
#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall

#pragma comment(lib, "libmarisa.lib")
typedef const char* BBStr;
using namespace opencc;

inline BBStr getCharPtr(std::string str) {
    char* cha = new char[str.size() + 1];
    memcpy(cha, str.c_str(), str.size() + 1);
    const char* p = cha;
    return p;
}


BLITZ3D(SimpleConverter*) CreateOpenCC(BBStr configFileName) {
    return new SimpleConverter(configFileName);
}

BLITZ3D(BBStr) OpenCConvert(SimpleConverter* converter, BBStr input) {
    return getCharPtr(converter->Convert(input));
}

BLITZ3D(void) FreeOpenCC(SimpleConverter* converter) {
    delete converter;
}