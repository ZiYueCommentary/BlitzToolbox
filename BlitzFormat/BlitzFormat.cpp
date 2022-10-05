/*
* BlitzFormat - A part of BlitzToolBox
* String formatter.
*
* v1.1 2022.9.29
*/

#include "../BlitzToolbox.hpp"
#include <format>

// very simple, but very useful! :D
// do not use std::format or you will get C7595

// i dont know why but this lib will get c++ runtime error when you do some complex format

template<typename... Args>
std::string format(const std::string fmt, Args... args) {
	return std::vformat(fmt, std::make_format_args(args...));
}

BLITZ3D(BBStr) Format(BBStr fmt, BBStr arg) {
	return getCharPtr(format(fmt, arg));
}

BLITZ3D(BBStr) Format2(BBStr fmt, BBStr arg1, BBStr arg2) {
	return getCharPtr(format(fmt, arg1, arg2));
}

BLITZ3D(BBStr) Format3(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3) {
	return getCharPtr(format(fmt, arg1, arg2, arg3));
}

BLITZ3D(BBStr) Format4(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4));
}

BLITZ3D(BBStr) Format5(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5));
}

BLITZ3D(BBStr) Format6(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5, BBStr arg6) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5, arg6));
}

BLITZ3D(BBStr) Format7(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5, BBStr arg6, BBStr arg7) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
}

BLITZ3D(BBStr) Format8(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5, BBStr arg6, BBStr arg7, BBStr arg8) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
}

BLITZ3D(BBStr) Format9(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5, BBStr arg6, BBStr arg7, BBStr arg8, BBStr arg9) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
}

BLITZ3D(BBStr) Format10(BBStr fmt, BBStr arg1, BBStr arg2, BBStr arg3, BBStr arg4, BBStr arg5, BBStr arg6, BBStr arg7, BBStr arg8, BBStr arg9, BBStr arg10) {
	return getCharPtr(format(fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10));
}