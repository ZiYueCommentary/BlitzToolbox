#define UNORDERED_MAP

#include "../IniControler/IniControler.cpp"
#include <atlstr.h>

BLITZ3D(BBStr) FindSCP294Drink(BBStr file, BBStr drink) {
	auto& buffer = IniBuffer[file];
	for (auto section = buffer.begin(); section != buffer.end(); section++) {
		vector<string> vec = BlitzToolbox::split_string(section->first, "|");
		for (string drink1 : vec) {
			CString drink2 = drink1.c_str();
			if (drink2.MakeLower() == CString(drink).MakeLower()) {
				return getCharPtr(section->first + ","s + drink1);
			}
		}
	}
	return "Null"; // nothing found ¯\_(ツ)_/¯
}