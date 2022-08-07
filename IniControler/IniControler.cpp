/*
* IniControler - A part of BlitzToolBox
* Write & Read ini file.
* 
* IniWriteString("options.ini", "foo", "bar", "wow")
* IniWriteBuffer("options.ini")
* IniGetString("options.ini", "foo", "bar")
*
* v1.0 2022.7.29
*/

#include <map>
#include <fstream>
#include <string>
#include <windows.h>

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall

using namespace std;
typedef const char* BBStr;

/*
* Buffer for ini files.
*/
map<BBStr, map<BBStr, BBStr>> IniBuffer;

BBStr getCharPtr(std::string str) {
    char* cha = new char[str.size() + 1];
    memcpy(cha, str.c_str(), str.size() + 1);
    const char* p = cha;
    return p;
}

/*
* Write buffer for ini file.
*
* @param path Path of ini file.
*/
BLITZ3D(void) IniWriteBuffer(BBStr path) {
    IniBuffer[path].clear();
    map<BBStr, BBStr> buffer;
    ifstream file(path);

    string line, section = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section = string(line, 1, line.length() - 2);
            continue;
        }
        if (line.find('=') != string::npos) {
            if (section == "") continue;
            string key = line.substr(0, line.find('='));
            if (key[key.length() - 1] == ' ') key = key.substr(0, key.length() - 1);
            string value = line.substr(line.find('=') + 1);
            if (value[0] == ' ') value = value.substr(1);
            buffer[(section + "," + key).c_str()] = getCharPtr(value);
        }
    }
    file.close();
    IniBuffer[path] = buffer;
}

/*
* Get value from ini file.
*
* @param path           Path of ini file.
* @param section        Section of key.
* @param key            Key of value.
* @param defaultValue   Value that if can't find value from ini file.
* @param allowBuffer    Allow IniControler get value from IniBuffer
*
* @return Value of key, or default value.
*/
BLITZ3D(BBStr) IniGetString(BBStr path, BBStr section, BBStr key, BBStr defaultValue, bool allowBuffer) {
    if (allowBuffer) { 
        if (IniBuffer[path][(string(section) + "," + key).c_str()] != nullptr) { // in java it will throw exception when get null
            return IniBuffer[path][(string(section) + "," + key).c_str()]; // but in c++ it wont throw
        }
    }

    ifstream file(path);
    string line, section1 = "";
    while (getline(file, line)) {
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section1 = string(line, 1, line.length() - 2);
            continue;
        }
        if (line.find('=') != string::npos) {
            if (section1 == "") continue;
            string key1 = line.substr(0, line.find('='));
            if (key1[key1.length() - 1] == ' ') key1 = key1.substr(0, key1.length() - 1);
            if ((section1 == section) && (key == key1)) {
                string value = line.substr(line.find('=') + 1);
                if (value[0] == ' ') value = value.substr(1);
                return getCharPtr(value);
            }
        }
    }
    return defaultValue;
}

BLITZ3D(int) IniGetInt(BBStr path, BBStr section, BBStr key, int defaultValue, bool allowBuffer) {
    return atoi(IniGetString(path, section, key, to_string(defaultValue).c_str(), allowBuffer));
}

BLITZ3D(float) IniGetFloat(BBStr path, BBStr section, BBStr key, float defaultValue, bool allowBuffer) {
    return atof(IniGetString(path, section, key, to_string(defaultValue).c_str(), allowBuffer));
}

/*
* Write something into ini file.
* 
* @param path           Path of ini file.
* @param section        Section of key.
* @param key            Key of value.
* @param value          Value of key.
* @param updateBuffer   Update value of buffer.
*/
BLITZ3D(void) IniWriteString(BBStr path, BBStr section, BBStr key, BBStr value, bool updateBuffer) {
    // very simple! :DDD
    WritePrivateProfileStringA(section, key, value, path);
    if (updateBuffer) IniBuffer[path][((std::string)section + "," + key).c_str()] = value;
}

BLITZ3D(void) IniWriteInt(BBStr path, BBStr section, BBStr key, int value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniWriteFloat(BBStr path, BBStr section, BBStr key, float value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}