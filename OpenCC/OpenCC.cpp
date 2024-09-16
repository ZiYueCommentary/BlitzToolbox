/*
* OpenCC - A part of BlitzToolbox
* Conversion between Traditional and Simplified Chinese.
*
* v1.01 2024.9.16
*/

#include "opencc/SimpleConverter.hpp"
#include "../BlitzToolbox.hpp"

using namespace opencc;

BLITZ3D(SimpleConverter*) CreateOpenCC(BBStr configFileName) {
	return new SimpleConverter(configFileName);
}

BLITZ3D(BBStr) OpenCConvert(SimpleConverter* converter, BBStr input) {
	return BlitzToolbox::getCharPtr(converter->Convert(input));
}

BLITZ3D(void) FreeOpenCC(SimpleConverter* converter) {
	delete converter;
}