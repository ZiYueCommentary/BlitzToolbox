/*
* BlitzEncode - A part of BlitzToolbox
* Encoding converter.
*
* v1.0 2022.9.23
*/

#include "../BlitzToolbox.hpp"
#include <atlstr.h>

void Converter(int src, int dst, CString& str) {
	uint32_t nLen = MultiByteToWideChar(src, NULL, str, -1, NULL, NULL);
	wchar_t* wszBuffer = new wchar_t[nLen + 1];
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

BLITZ3D(int) GetCodePage() {
	return GetACP();
}

BLITZ3D(BBStr) ConvertEncoding(BBStr str, int sourceCodePage, int destCodePage) {
	CString cstr = str; // dont delete this or you will get crash
	Converter(sourceCodePage, destCodePage, cstr);
	return getCharPtr(cstr.GetBuffer());
}