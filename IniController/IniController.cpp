/*
* IniController - A part of BlitzToolbox
* Reading & writing INI files.
* 
* v1.10 2026.02.23
*/

#include "resource.h"
#include "../BlitzToolbox.hpp"
#include <fstream>
#include <absl/container/flat_hash_map.h>

#ifdef NORMALIZE_PATH
#define NORMALIZE_PATH(path) BlitzToolbox::normalize_path(std::string(path))
#else
#define NORMALIZE_PATH(path) std::string(path)
#endif

template <typename T, typename V>
using IniMap = absl::flat_hash_map<T, V>;

static IniMap<std::string, IniMap<std::string, IniMap<std::string, std::string>>> IniBuffer{};

BLITZ3D(void) IniWriteBuffer(BBStr path, BBStr bufferPath, int clearPrevious) {
	const std::string new_buffer_path = strcmp(bufferPath, "") ? NORMALIZE_PATH(path) : bufferPath;
	if (clearPrevious) IniBuffer[new_buffer_path].clear();
	std::ifstream file(path);
	if (!file.is_open()) return;

	std::string line, section;
	while (std::getline(file, line)) {
		if (line[0] == ';') continue;
		if (line[0] == '[' && line[line.length() - 1] == ']') {
			section = std::string(line, 1, line.length() - 2);
			continue;
		}
		if (line.find('=') != std::string::npos) {
			if (section.empty()) continue;
			std::string key = line.substr(0, line.find('='));
			if (key[key.length() - 1] == ' ') key = key.substr(0, key.length() - 1);
			std::string value = line.substr(line.find('=') + 1);
			if (value[0] == ' ') value = value.substr(1);
			IniBuffer[new_buffer_path][section][key] = value;
		}
	}
	file.close();
}

/* Read INI */

BLITZ3D(BBStr) IniGetString(BBStr path, BBStr section, BBStr key, BBStr defaultValue, int allowBuffer) {
	if (allowBuffer) {
		if (IniBuffer[NORMALIZE_PATH(path)][section].contains(key)) {
			return IniBuffer[NORMALIZE_PATH(path)][section][key].c_str();
		}
	}

	std::ifstream file(path);
	std::string line, section1;
	while (std::getline(file, line)) {
		if (line[0] == ';') continue;
		if (line[0] == '[' && line[line.length() - 1] == ']') {
			section1 = std::string(line, 1, line.length() - 2);
			continue;
		}
		if (line.find('=') != std::string::npos) {
			if (section1.empty()) continue;
			std::string key1 = line.substr(0, line.find('='));
			if (key1[key1.length() - 1] == ' ') key1 = key1.substr(0, key1.length() - 1);
			if ((section1 == section) && (key == key1)) {
				std::string value = line.substr(line.find('=') + 1);
				if (value[0] == ' ') value = value.substr(1);
				return BlitzToolbox::blitz3d_string(value);
			}
		}
	}
	file.close();
	return defaultValue;
}

BLITZ3D(int) IniGetInt(BBStr path, BBStr section, BBStr key, int defaultValue, int allowBuffer) {
	return atoi(IniGetString(path, section, key, std::to_string(defaultValue).c_str(), allowBuffer));
}

BLITZ3D(float) IniGetFloat(BBStr path, BBStr section, BBStr key, float defaultValue, int allowBuffer) {
	return atof(IniGetString(path, section, key, std::to_string(defaultValue).c_str(), allowBuffer));
}

BLITZ3D(int) IniSectionExist(BBStr path, BBStr section, int allowBuffer) {
	if (allowBuffer) {
		if (const int contain = IniBuffer[NORMALIZE_PATH(path)].contains(section)) return contain;
	}

	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == ';') continue;
		if (line[0] == '[' && line[line.length() - 1] == ']') {
			std::string section1 = std::string(line, 1, line.length() - 2);
			if (section == section1) return true;
		}
	}
	file.close();
	return false;
}

BLITZ3D(int) IniBufferSectionExist(BBStr path, BBStr section) {
	return IniBuffer[NORMALIZE_PATH(path)].contains(section);
}

BLITZ3D(int) IniKeyExist(BBStr path, BBStr section, BBStr key, int allowBuffer) {
	if (allowBuffer) {
		if (IniBuffer[NORMALIZE_PATH(path)][section].contains(key)) return 1;
	}

	std::ifstream file(path);
	std::string line, section1;
	while (std::getline(file, line)) {
		if (line[0] == ';') continue;
		if (line[0] == '[' && line[line.length() - 1] == ']') {
			section1 = std::string(line, 1, line.length() - 2);
			continue;
		}
		if (line.find('=') != std::string::npos) {
			if (section1.empty()) continue;
			std::string key1 = line.substr(0, line.find('='));
			if (key1[key1.length() - 1] == ' ') key1 = key1.substr(0, key1.length() - 1);
			if (section1 == section && key == key1) return true;
		}
	}
	file.close();
	return false;
}

BLITZ3D(int) IniBufferKeyExist(BBStr path, BBStr section, BBStr key) {
	return IniBuffer[NORMALIZE_PATH(path)][section].contains(key);
}

BLITZ3D(BBStr) IniGetBufferString(BBStr path, BBStr section, BBStr key, BBStr defaultValue) {
	if (IniBuffer[NORMALIZE_PATH(path)][section].contains(key))
		return IniBuffer[NORMALIZE_PATH(path)][section][key].c_str();

	return defaultValue;
}

BLITZ3D(int) IniGetBufferInt(BBStr path, BBStr section, BBStr key, int defaultValue) {
	return atoi(IniGetBufferString(path, section, key, std::to_string(defaultValue).c_str()));
}

BLITZ3D(float) IniGetBufferFloat(BBStr path, BBStr section, BBStr key, float defaultValue) {
	return atof(IniGetBufferString(path, section, key, std::to_string(defaultValue).c_str()));
}

/* Write INI */

BLITZ3D(void) IniWriteString(BBStr path, BBStr section, BBStr key, BBStr value, int updateBuffer) {
	// maybe i will write one by my self but im too lazy lol
	WritePrivateProfileStringA(section, key, value, NORMALIZE_PATH(path).c_str());
	if (updateBuffer) IniBuffer[NORMALIZE_PATH(path)][section][key] = value;
}

BLITZ3D(void) IniWriteInt(BBStr path, BBStr section, BBStr key, int value, int updateBuffer) {
	IniWriteString(path, section, key, std::to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniWriteFloat(BBStr path, BBStr section, BBStr key, float value, int updateBuffer) {
	IniWriteString(path, section, key, std::to_string(value).c_str(), updateBuffer);
}

BLITZ3D(void) IniRemoveKey(BBStr path, BBStr section, BBStr key, int updateBuffer) {
	WritePrivateProfileStringA(section, key, NULL, std::filesystem::absolute(path).generic_string().c_str());
	if (updateBuffer) IniBuffer[NORMALIZE_PATH(path)][section].erase(key);
}

BLITZ3D(void) IniCreateSection(BBStr path, BBStr section) {
	WritePrivateProfileSectionA(section, "", std::filesystem::absolute(path).generic_string().c_str());
}

BLITZ3D(void) IniRemoveSection(BBStr path, BBStr section, int updateBuffer) {
	WritePrivateProfileSectionA(section, NULL, std::filesystem::absolute(path).generic_string().c_str());
	if (updateBuffer) IniBuffer[NORMALIZE_PATH(path)].erase(section);
}

BLITZ3D(void) IniRemoveBufferKey(BBStr path, BBStr section, BBStr key) {
	IniBuffer[NORMALIZE_PATH(path)][section].erase(key);
}

BLITZ3D(void) IniRemoveBufferSection(BBStr path, BBStr section) {
	IniBuffer[NORMALIZE_PATH(path)].erase(section);
}

/* BUFFER */

BLITZ3D(void) IniClearBuffer(BBStr path) {
	IniBuffer[NORMALIZE_PATH(path)].clear();
}

BLITZ3D(void) IniClearAllBuffer() {
	IniBuffer.clear();
}

/* DANGER ZONE */

BLITZ3D(void) IniSetBufferValue(BBStr path, BBStr section, BBStr key, BBStr value) {
	IniBuffer[NORMALIZE_PATH(path)][section][key] = value;
}

BLITZ3D(void) IniSetExportBufferValue(IniMap<std::string, IniMap<std::string, std::string>>* buffer, BBStr section, BBStr key, BBStr value) {
	(*buffer)[section][key] = value;
}

extern "C" __declspec(dllexport) IniMap<std::string, IniMap<std::string, std::string>>*_stdcall IniGetBuffer(BBStr path) {
	return &IniBuffer[NORMALIZE_PATH(path)];
}

extern "C" __declspec(dllexport) IniMap<std::string, IniMap<std::string, IniMap<std::string, std::string>>>*_stdcall IniGetAllBuffer() {
	return &IniBuffer;
}

BLITZ3D(void) IniSetBuffer(BBStr path, IniMap<std::string, IniMap<std::string, std::string>>* buffer) {
	IniBuffer[NORMALIZE_PATH(path)] = *buffer;
}

BLITZ3D(void) IniSetAllBuffer(IniMap<std::string, IniMap<std::string, IniMap<std::string, std::string>>>* buffer) {
	IniBuffer = *buffer;
}

/* EXPORT */
IniMap<std::string, IniMap<std::string, std::string>>* GetIniMap(BBStr path, int allowBuffer) {
	IniMap<std::string, IniMap<std::string, std::string>>* buffer;
	if (allowBuffer && IniBuffer.contains(path)) {
		buffer = &IniBuffer[NORMALIZE_PATH(path)];
	}
	else {
		buffer = new IniMap<std::string, IniMap<std::string, std::string>>();
		std::ifstream file(path);
		if (!file.is_open()) return buffer;

		std::string line, section;
		while (std::getline(file, line)) {
			if (line[0] == ';') continue;
			if (line[0] == '[' && line[line.length() - 1] == ']') {
				section = std::string(line, 1, line.length() - 2);
				continue;
			}
			if (line.find('=') != std::string::npos) {
				if (section.empty()) continue;
				std::string key = line.substr(0, line.find('='));
				if (key[key.length() - 1] == ' ') key = key.substr(0, key.length() - 1);
				std::string value = line.substr(line.find('=') + 1);
				if (value[0] == ' ') value = value.substr(1);
				(*buffer)[section][key] = value;
			}
		}
		file.close();
	}
	return buffer;
}

/* INI */
void ExportIni(IniMap<std::string, IniMap<std::string, std::string>>&& sectionBuffer, BBStr path, int isMin) {
	std::ofstream ini(path);
	const char* space = isMin ? "" : " ";
	for (auto section = sectionBuffer.begin(); section != sectionBuffer.end(); ++section) {
		IniMap<std::string, std::string>& keyBuffer = sectionBuffer[section->first];
		ini << "[" << section->first << "]" << '\n';
		for (auto key = keyBuffer.begin(); key != keyBuffer.end(); ++key) {
			ini << key->first << space << "=" << space << keyBuffer[key->first] << '\n';
		}
		if (!isMin) ini << '\n';
	}
	ini.close();
}

BLITZ3D(void) IniExportIni(BBStr path, BBStr ini, int isMin, int allowBuffer) {
	ExportIni(std::move(*GetIniMap(path, allowBuffer)), ini, isMin);
}

BLITZ3D(void) IniBufferExportIni(BBStr path, BBStr ini, int isMin) {
	ExportIni(std::move(IniBuffer[NORMALIZE_PATH(path)]), ini, isMin);
}