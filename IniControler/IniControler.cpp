/*
* IniControler - A part of BlitzToolBox
* Write & Read ini file.
* 
* v1.02 2022.8.18
*/

#include "../BlitzToolbox.hpp"
#include <map>
#include <fstream>
#include <windows.h>

using namespace std;

// We can't use const char* to get things from buffer (maybe because string's pointer is different?)
// so we must use std::string to get things from buffer
/*
* Buffer for ini files.
*/
static map<string, map<string, map<string, BBStr>>> IniBuffer;

/*
* Write buffer for ini file.
*
* @param path          Path of ini file.
* @param clearPervious Clear old content of buffer.
*/
BLITZ3D(void) IniWriteBuffer(BBStr path, bool clearPrevious) {
    if(clearPrevious) IniBuffer[path].clear();
    map<string, map<string, BBStr>> buffer;
    ifstream file(path);
    if (!file.is_open()) return;

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
            buffer[section][key] = getCharPtr(value);
        }
    }
    file.close();
    IniBuffer[path] = buffer;
}

/* Read INI */

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
        if (IniBuffer[path][section].contains(key)) { // in java it will throw exception when get null
            return IniBuffer[path][section][key]; // but in c++ it wont throw
        }
    }

    ifstream file(path);
    string line, section1 = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
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

BLITZ3D(bool) IniSectionExist(BBStr path, BBStr section, bool allowBuffer) {
    if (allowBuffer) {
        bool contain = IniBuffer[path].contains(section);
        if (contain) return contain;
    }

    ifstream file(path);
    string line, section1 = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section1 = string(line, 1, line.length() - 2);
            if (section == section1) return true;
        }
        continue;
    }
    return false;
}

BLITZ3D(bool) IniBufferSectionExist(BBStr path, BBStr section) {
    return IniBuffer[path].contains(section);
}

BLITZ3D(bool) IniKeyExist(BBStr path, BBStr section, BBStr key, bool allowBuffer) {
    if (allowBuffer) {
        bool contain = IniBuffer[path][section].contains(key);
        if (contain) return contain;
    }

    ifstream file(path);
    string line, section1 = "";
    while (getline(file, line)) {
        if (line[0] == ';') continue;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section1 = string(line, 1, line.length() - 2);
            continue;
        }
        if (line.find('=') != string::npos) {
            if (section1 == "") continue;
            string key1 = line.substr(0, line.find('='));
            if (key1[key1.length() - 1] == ' ') key1 = key1.substr(0, key1.length() - 1);
            if ((section1 == section) && (key == key1)) return true;
        }
    }
    return false;
}

BLITZ3D(bool) IniBufferKeyExist(BBStr path, BBStr section, BBStr key) {
    return IniBuffer[path][section].contains(key);
}

/*
* Get value from buffer ONLY.
*
* @param path           Path of ini file.
* @param section        Section of key.
* @param key            Key of value.
* @param defaultValue   Value that if can't find value from buffer.
*
* @return Value of key, or default value.
*/
BLITZ3D(BBStr) IniGetBufferString(BBStr path, BBStr section, BBStr key, BBStr defaultValue) {
    if (IniBuffer[path][section].contains(key))
        return IniBuffer[path][section][key];
    else 
        return defaultValue;
}

BLITZ3D(int) IniGetBufferInt(BBStr path, BBStr section, BBStr key, int defaultValue) {
    return atoi(IniGetBufferString(path, section, key, to_string(defaultValue).c_str()));
}

BLITZ3D(float) IniGetBufferFloat(BBStr path, BBStr section, BBStr key, float defaultValue) {
    return atof(IniGetBufferString(path, section, key, to_string(defaultValue).c_str()));
}

/* Write INI */

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
    // maybe i will write one by my self but im too lazy lol
    WritePrivateProfileStringA(section, key, value, path);
    if (updateBuffer) IniBuffer[path][section][key] = value;
}

BLITZ3D(void) IniWriteInt(BBStr path, BBStr section, BBStr key, int value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniWriteFloat(BBStr path, BBStr section, BBStr key, float value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}

/* BUFFER */

/*
* Clear buffer of path.
* 
* @param path name of buffer.
*/
BLITZ3D(void) IniClearBuffer(BBStr path) {
    IniBuffer[path].clear();
}

/*
* Clear all buffer.
*/
BLITZ3D(void) IniClearAllBuffer() {
    IniBuffer.clear();
}

/* DANGER ZONE */

/*
* Set buffer's value.
* 
* @param path           Path of ini file.
* @param section        Section of key.
* @param key            Key of value.
* @param value          Value of key.
*/
BLITZ3D(void) IniSetBufferValue(BBStr path, BBStr section, BBStr key, BBStr value) {
    IniBuffer[path][section][key] = value;
}

extern "C" __declspec(dllexport) map<string, map<string, BBStr>>* _stdcall IniGetBuffer(BBStr path) {
    return &IniBuffer[path];
}

extern "C" __declspec(dllexport) map<string, map<string, map<string, BBStr>>>* _stdcall IniGetAllBuffer() {
    return &IniBuffer;
}

BLITZ3D(void) IniSetBuffer(BBStr path, map<string, map<string, BBStr>>* buffer) {
    IniBuffer[path] = *buffer;
}

BLITZ3D(void) IniSetAllBuffer(map<string, map<string, map<string, BBStr>>>* buffer) {
    IniBuffer = *buffer;
}