//developing

/*
* IniControler - A part of BlitzToolBox
* Write & Read ini file.
* 
* v1.0 2022.7.29
*/

#include <map>
#include <fstream>
#include <string>
#include <iostream>

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall

using namespace std;
typedef const char* BBStr;

/*
* Buffer for ini files.
*/
map<BBStr, map<BBStr, BBStr>> IniBuffer;

/*
* Write buffer for ini file.
* 
* @param path   Path of ini file.
*/
BLITZ3D(void) IniWriteBuffer(BBStr path) {
    map<BBStr, BBStr> buffer;
    ifstream file(path);

    string line, section = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section = line.substr(1).substr(0, line.length() - 2);
            continue;
        }
        if (line.find('=') != string::npos) {
            if (section == "") continue;
            string key = line.substr(0, line.find('=') - 1);
            if (key[line.length() - 1] = ' ') key = key.substr(0, line.length() - 2);
            string value = line.substr(line.find('=') + 1);
            if (value[0] == ' ') value = value.substr(1);
            buffer[(section + "," + key).c_str()] = value.c_str();
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
    try {
        if (allowBuffer) return IniBuffer[path][(string(section) + "," + key).c_str()];
    } catch(exception) {}
    ifstream file(path);

    string line, section1 = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section1 = line.substr(1).substr(0, line.length() - 2);
            continue;
        }
        if (line.find('=') != string::npos) {
            if (section1 == "") continue;
            string key1 = line.substr(0, line.find('=') - 1);
            if (key1[line.length() - 1] = ' ') key1 = key1.substr(0, line.length() - 2);
            cout << key1;
            if ((section1 == section) && (key == key1)) {
                string value = line.substr(line.find('=') + 1);
                if (value[0] == ' ') value = value.substr(1);
                file.close();
                return value.c_str();
            }
        }
    }

    file.close();
    return defaultValue;
}

BLITZ3D(int) IniGetInt(BBStr path, BBStr section, BBStr key, int defaultValue, bool allowBuffer) {
    return atoi(IniGetString(path, section, key, to_string(defaultValue).c_str(), allowBuffer));
}

BLITZ3D(float) IniGetFloat(BBStr path, BBStr section, BBStr key, float defaultValue, bool allowBuffer) {
    return atof(IniGetString(path, section, key, to_string(defaultValue).c_str(), allowBuffer));
}

int main() {
    IniGetString("C:\\Users\\myhom\\source\\repos\\BlitzToolbox\\IniControler\\foo.ini", "foo", "bar", "114514", 0);
}