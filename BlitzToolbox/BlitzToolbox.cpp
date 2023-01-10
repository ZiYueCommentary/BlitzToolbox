#define UNORDERED_MAP // quicker!
// less header, less file size

#include "../IniControler/IniControler.cpp"

inline string MakeLower(string str) {
	// in scpcb-ue, input of scp294 are always pure english
	string ret = "";
	for (char c : str) {
		ret += char(tolower(c));
	}
	return ret;
}

BLITZ3D(BBStr) FindSCP294Drink(BBStr file, BBStr drink) {
	auto& buffer = IniBuffer[file];
	for (auto section = buffer.begin(); section != buffer.end(); section++) {
		vector<string> vec = BlitzToolbox::split_string(section->first, "|");
		for (string drink1 : vec) {
			if (MakeLower(drink1) == MakeLower(drink)) {
				return getCharPtr(section->first + ","s + drink1);
			}
		}
	}
	return "Null"; // nothing found ¯\_(ツ)_/¯
}