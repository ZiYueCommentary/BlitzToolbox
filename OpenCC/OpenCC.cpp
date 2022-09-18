/*
* OpenCC - A part of BlitzToolbox
* Conversion between Traditional and Simplified Chinese.
* 
* v1.0 2022.7.27
*/

#include "opencc/SimpleConverter.hpp"
#include "../BlitzToolbox.hpp"

using namespace opencc;

BLITZ3D(SimpleConverter*) CreateOpenCC(BBStr configFileName) {
    return new SimpleConverter(configFileName);
}

BLITZ3D(BBStr) OpenCConvert(SimpleConverter* converter, BBStr input) {
    return getCharPtr(converter->Convert(input));
}

BLITZ3D(void) FreeOpenCC(SimpleConverter* converter) {
    delete converter;
}