/*
* IniControler - A part of BlitzToolbox
* Write & Read ini file.
*
* v1.06 2022.11.12
*/

#include "resource.h"
#include "../BlitzToolbox.hpp"
#include <fstream>
#include <windows.h>
#include <filesystem>

#ifdef UNORDERED_MAP // see "resource.h"
#include <unordered_map>
template <typename T, typename V>
using IniMap = std::unordered_map<T, V>;
#else
#include <map>
template <typename T, typename V>
using IniMap = std::map<T, V>;
#endif

using namespace std;

// We can't use const char* to get things from buffer (maybe because string's pointer is different?)
// so we must use std::string to get things from buffer
/*
* Buffer for ini files.
*/
static IniMap<string, IniMap<string, IniMap<string, string>>> IniBuffer;

/*
* Write buffer for ini file.
*
* @param path          Path of ini file.
* @param clearPervious Clear old content of buffer.
*/
BLITZ3D(void) IniWriteBuffer(BBStr path, bool clearPrevious) {
    if (clearPrevious) IniBuffer[path].clear();
    IniMap<string, IniMap<string, string>> buffer;
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
            buffer[section][key] = value;
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
            return IniBuffer[path][section][key].c_str(); // but in c++ it wont throw
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
    file.close();
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
    }
    file.close();
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
    file.close();
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
        return IniBuffer[path][section][key].c_str();
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
    // maybe i will write one by my self but im too lazy lol
    WritePrivateProfileStringA(section, key, value, filesystem::absolute(path).generic_string().c_str());
    if (updateBuffer) IniBuffer[path][section][key] = value;
}

BLITZ3D(void) IniWriteInt(BBStr path, BBStr section, BBStr key, int value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniWriteFloat(BBStr path, BBStr section, BBStr key, float value, bool updateBuffer) {
    IniWriteString(path, section, key, to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniRemoveKey(BBStr path, BBStr section, BBStr key, bool updateBuffer) {
    WritePrivateProfileStringA(section, key, NULL, filesystem::absolute(path).generic_string().c_str());
    if (updateBuffer) IniBuffer[path][section].erase(key);
}

BLITZ3D(void) IniCreateSection(BBStr path, BBStr section) {
    WritePrivateProfileSectionA(section, "", filesystem::absolute(path).generic_string().c_str());
}

BLITZ3D(void) IniRemoveSection(BBStr path, BBStr section, bool updateBuffer) {
    WritePrivateProfileSectionA(section, NULL, filesystem::absolute(path).generic_string().c_str());
    if (updateBuffer) IniBuffer[path].erase(section);
}

BLITZ3D(void) IniRemoveBufferKey(BBStr path, BBStr section, BBStr key) {
    IniBuffer[path][section].erase(key);
}

BLITZ3D(void) IniRemoveBufferSection(BBStr path, BBStr section) {
    IniBuffer[path].erase(section);
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

BLITZ3D(void) IniSetExportBufferValue(IniMap<string, IniMap<string, string>>* buffer, BBStr section, BBStr key, BBStr value) {
    (*buffer)[section][key] = value;
}

extern "C" __declspec(dllexport) IniMap<string, IniMap<string, string>>* _stdcall IniGetBuffer(BBStr path) {
    return &IniBuffer[path];
}

extern "C" __declspec(dllexport) IniMap<string, IniMap<string, IniMap<string, string>>>* _stdcall IniGetAllBuffer() {
    return &IniBuffer;
}

BLITZ3D(void) IniSetBuffer(BBStr path, IniMap<string, IniMap<string, string>>* buffer) {
    IniBuffer[path] = *buffer;
}

BLITZ3D(void) IniSetAllBuffer(IniMap<string, IniMap<string, IniMap<string, string>>>* buffer) {
    IniBuffer = *buffer;
}

/* EXPORT */

IniMap<string, IniMap<string, string>>* GetIniMap(BBStr path, bool allowBuffer) {
    IniMap<string, IniMap<string, string>>* buffer;
    if (allowBuffer && IniBuffer.contains(path)) {
        buffer = &IniBuffer[path];
    }
    else {
        buffer = new IniMap<string, IniMap<string, string>>();
        ifstream file(path);
        if (!file.is_open()) return buffer;

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
                (*buffer)[section][key] = value;
            }
        }
        file.close();
    }
    return buffer;
}

/* INI */
void ExportIni(IniMap<string, IniMap<string, string>>&& sectionBuffer, BBStr path, bool isMin) {
    ofstream ini(path);
    const char* space = isMin ? "" : " ";
    for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); section++) {
        IniMap<string, string>& keyBuffer = sectionBuffer[section->first];
        ini << "[" << section->first << "]" << endl;
        for (auto key = keyBuffer.begin(); key != keyBuffer.end(); key++) {
            ini << key->first << space << "=" << space << keyBuffer[key->first] << endl;
        }
        if (!isMin) ini << endl;
    }
    ini.close();
}

BLITZ3D(void) IniExportIni(BBStr path, BBStr ini, bool isMin, bool allowBuffer) {
    ExportIni(std::move(*GetIniMap(path, allowBuffer)), ini, isMin);
}

BLITZ3D(void) IniBufferExportIni(BBStr path, BBStr ini, bool isMin) {
    ExportIni(std::move(IniBuffer[path]), ini, isMin);
}

/* JSON */
// i dont know what is "rvalue reference" but i will have a try
void ExportJson(IniMap<string, IniMap<string, string>>&& sectionBuffer, BBStr path, bool isMin, bool stringOnly) {
    ofstream json(path);
    const char* endl = isMin ? "" : "\n";
    const char* indent = isMin ? "" : "    ";
    const char* space = isMin ? "" : " ";
    json << "{" << endl;
    for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); section++) {
        json << indent << "\"" << BlitzToolbox::json_friendly_string(section->first) << "\":" << space << "{" << endl;
        IniMap<string, string>& keyBuffer = sectionBuffer[section->first];
        for (auto key = keyBuffer.begin(); key != keyBuffer.end(); key++) {
            string value = BlitzToolbox::json_friendly_string(keyBuffer[key->first]);
            if (stringOnly) {
                value = "\"" + value + "\"";
            }
            else if (value == "true" || value == "True") {
                value = "true";
            }
            else if (value == "false" || value == "False") {
                value = "false";
            }
            else if (value == to_string(atoi(value.c_str()))) {
            }
            else if (value == to_string(atof(value.c_str())).substr(0, value.length())) {
            }
            else {
                value = "\"" + value + "\"";
            }
            json << indent << indent << "\"" << BlitzToolbox::json_friendly_string(key->first) << "\":" << space << value << (key == --keyBuffer.end() ? "" : ",") << endl;
        }
        json << indent << "}" << (section == --sectionBuffer.end() ? "" : ",") << endl;
    }
    json << "}";
    json.close();
}

BLITZ3D(void) IniExportJson(BBStr path, BBStr json, bool isMin, bool stringOnly, bool allowBuffer) {
    ExportJson(std::move(*GetIniMap(path, allowBuffer)), json, isMin, stringOnly);
}

BLITZ3D(void) IniBufferExportJson(BBStr path, BBStr json, bool isMin, bool stringOnly) {
    ExportJson(std::move(IniBuffer[path]), json, isMin, stringOnly);
}

/* HTML */
void ExportHtml(IniMap<string, IniMap<string, string>>&& sectionBuffer, BBStr file, BBStr path, bool isMin, bool isList) {
    ofstream html(path);
    const char* endl = isMin ? "" : "\n";
    const char* indent = isMin ? "" : "    ";
    const string filename = filesystem::path(file).filename().generic_string();
    // writing head
    html << "<head>" << endl;
    html << indent << "<title>Index of " << filename << "</title>" << endl;
    html << "</head>" << endl;
    html << endl;
    html << "<html>" << endl;
    html << indent << "<h1>Index of " << filename << "</h1>" << endl;
    html << endl;
    if (isList) { // as list
        html << indent << "<ul>";
        for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); section++) {
            html << endl;
            html << indent << indent << "<li>" << endl;
            html << indent << indent << indent << "<b>" << BlitzToolbox::html_friendly_string(section->first) << "</b>" << endl;
            html << indent << indent << indent << "<ul>" << endl;
            IniMap<string, string>& keyBuffer = sectionBuffer[section->first];
            for (auto key = keyBuffer.begin(); key != keyBuffer.end(); key++) {
                html << indent << indent << indent << indent << "<li><i>" << BlitzToolbox::html_friendly_string(key->first) << "</i> - " << BlitzToolbox::html_friendly_string(keyBuffer[key->first]) << "</li>" << endl;
            }
            html << indent << indent << indent << "</ul>" << endl;
            html << indent << indent << "</li>";
        }
        html << endl;
        html << indent << "</ul>" << endl;
    }
    else { // as table
        html << indent << "<table width=\"100%\" border=\"1\">" << endl;
        html << indent << indent << "<tr>" << endl;
        html << indent << indent << indent << "<th>Section</th>" << endl;
        html << indent << indent << indent << "<th>Key</th>" << endl;
        html << indent << indent << indent << "<th>Value</th>" << endl;
        html << indent << indent << "</tr>" << endl;
        for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); section++) {
            html << indent << indent << "<tr>" << endl;
            IniMap<string, string>& keyBuffer = sectionBuffer[section->first];
            html << indent << indent << indent << "<td rowspan=\"" << keyBuffer.size() << "\">" << BlitzToolbox::html_friendly_string(section->first) << "</td>" << endl;
            for (auto key = keyBuffer.begin(); key != keyBuffer.end(); key++) {
                if (key != keyBuffer.begin())
                    html << indent << indent << "<tr>" << endl;
                html << indent << indent << indent << "<td>" << BlitzToolbox::html_friendly_string(key->first) << "</td>" << endl;
                html << indent << indent << indent << "<td>" << BlitzToolbox::html_friendly_string(keyBuffer[key->first]) << "</td>" << endl;
                html << indent << indent << "</tr>" << endl;
            }
        }
        html << indent << "</table>" << endl;
    }
    html << indent << "Generate by <i>IniControler</i> of <a href=\"https://github.com/ZiYueCommentary/BlitzToolbox\" target=\"_blank\">BlitzToolbox</a>.";
    html << endl;
    html << "</html>";
    html.close();
}

BLITZ3D(void) IniExportHtml(BBStr path, BBStr html, bool isMin, bool isList, bool allowBuffer) {
    ExportHtml(std::move(*GetIniMap(path, allowBuffer)), path, html, isMin, isList);
}

BLITZ3D(void) IniBufferExportHtml(BBStr path, BBStr html, bool isMin, bool isList) {
    ExportHtml(std::move(IniBuffer[path]), path, html, isMin, isList);
}

/* XML */
void ExportXml(IniMap<string, IniMap<string, string>>&& sectionBuffer, BBStr file, BBStr path, bool isMin) {
    ofstream xml(path);
    const char* endl = isMin ? "" : "\n";
    const char* indent = isMin ? "" : "    ";
    string filename = filesystem::path(file).filename().generic_string();
    const string filexten = filesystem::path(file).filename().extension().generic_string();
    filename = filename.substr(filename.rfind(filexten));
    xml << "<?xml version=\"1.0\"?>" << endl;
    if (!isMin) {
        xml << "<!--" << endl;
        xml << indent << "Generate by \"IniControler\" of BlitzToolbox." << endl;
        xml << indent << "https://github.com/ZiYueCommentary/BlitzToolbox" << endl;
        xml << "-->" << endl << endl;
    }
    xml << "<file name=\"" << BlitzToolbox::json_friendly_string(filename) << "\">" << endl; // json friendly == " -> \"
    for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); section++) {
        xml << indent << "<section>" << endl;
        xml << indent << indent << "<name>" << BlitzToolbox::xml_friendly_string(section->first) << "</name>" << endl;
        IniMap<string, string>& keyBuffer = sectionBuffer[section->first];
        for (auto key = keyBuffer.begin(); key != keyBuffer.end(); key++) {
            xml << indent << indent << "<key>" << endl;
            xml << indent << indent << indent << "<name>" << BlitzToolbox::xml_friendly_string(key->first) << "</name>" << endl;
            xml << indent << indent << indent << "<value>" << BlitzToolbox::xml_friendly_string(keyBuffer[key->first]) << "</value>" << endl;
            xml << indent << indent << "</key>" << endl;
        }
        xml << indent << "</section>" << endl;
    }
    xml << "</file>" << endl;
    xml.close();
}

BLITZ3D(void) IniExportXml(BBStr path, BBStr xml, bool isMin, bool allowBuffer) {
    ExportXml(std::move(*GetIniMap(path, allowBuffer)), path, xml, isMin);
}

BLITZ3D(void) IniBufferExportXml(BBStr path, BBStr xml, bool isMin) {
    ExportXml(std::move(IniBuffer[path]), path, xml, isMin);
}
