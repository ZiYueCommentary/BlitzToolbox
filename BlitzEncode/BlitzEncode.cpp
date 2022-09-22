/*
* BlitzEncode - A part of BlitzToolbox
* Encoding converter.
* 
* v1.0 2022.9.22
*/

#include "../BlitzToolbox.hpp"
#include <atlstr.h>

void Converter(int src, int dst, CString& str) {
    UINT nLen = MultiByteToWideChar(src, NULL, str, -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(src, NULL, str, -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(dst, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    char* szBuffer = new char[nLen + 1];
    nLen = WideCharToMultiByte(dst, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    str = szBuffer;
    delete[] szBuffer;
    delete[] wszBuffer;
}

BLITZ3D(int) GetPageCode() {
	return GetACP();
}

BLITZ3D(BBStr) ConvertEncoding(BBStr str, int sourcePageCode, int destPageCode) {
    CString cstr = str;
    Converter(sourcePageCode, destPageCode, cstr);
    std::string result = cstr.GetBuffer();
    delete cstr;
    return getCharPtr(result);
}