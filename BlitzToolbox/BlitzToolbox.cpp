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

BLITZ3D(BBStr) FindNextDirectory(BBStr path, BBStr directory) {
    WIN32_FIND_DATAA fdFindData;
    HANDLE hFind;
    std::string filename = path;
    filename += "\\*.*";
    int count = 0;
    BOOL done;
    BOOL nextfolder = !std::filesystem::exists(std::filesystem::path(std::string(path) + "\\"s + directory));
    hFind = FindFirstFileA(filename.c_str(), &fdFindData);
    done = hFind != INVALID_HANDLE_VALUE;
    while (done)
    {
        if (strcmp(fdFindData.cFileName, ".") && strcmp(fdFindData.cFileName, ".."))
        {
            filename = path;
            filename += "\\"s + fdFindData.cFileName;
            if ((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
                if (nextfolder) return getCharPtr(BlitzToolbox::replace_all(filename, std::string(path) + "\\", ""));
                if (filename == std::string(path) + "\\"s + directory) nextfolder = TRUE;
            }
        }
        done = FindNextFileA(hFind, &fdFindData);
    }
    FindClose(hFind);
    return "en";
}