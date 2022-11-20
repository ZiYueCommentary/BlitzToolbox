#define UNORDERED_MAP

#include "../IniControler/IniControler.cpp"

BLITZ3D(BBStr) FindSCP294Drink(BBStr file, BBStr drink) {
	for (auto drink1 : IniBuffer[file]) { // crazy for each
		vector<string> drinks = BlitzToolbox::split_string(drink1.first, "|");
		for (string drink2 : drinks) {
			if (drink2 == drink) return getCharPtr(drink1.first);
		}
	}
	return "Null"; // nope :(
}