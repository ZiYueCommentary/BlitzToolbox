/*
* Header file of BlitzToolbox.
*/

#include <string>

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall
typedef const char* BBStr;

BBStr getCharPtr(std::string str) {
    char* cha = new char[str.size() + 1];
    memcpy(cha, str.c_str(), str.size() + 1);
    const char* p = cha;
    return p;
}
