/*
* RapidBson - A part of BlitzToolbox
* A fast JSON parser/generator for Blitz3D with both SAX/DOM style API.
*
* v1.0 2024.10.3
*/

///////////////////////////
/// IN DEVELOPING STAGE ///
///////////////////////////

#define BLITZ3DTSS

#define DOCUMENT_IDENT "_RapidBson@DocObj"
#define SAFE_TYPE(value, type) (value)->Is##type() ? (value)->Get##type() : NULL

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include "../BlitzToolbox.hpp"
#include <functional>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        printf_s("RapidBson: In the development stage. May not be stable or useable. \n");
    }
    return TRUE;
}

using namespace rapidjson;
using Array = GenericValue<UTF8<>>::Array;

enum class VariableTypes
{
    Document, Value, Array, Null
};

struct JsonVariable {
    VariableTypes type;
    union {
        Document* JsonDocument = nullptr;
        Value* JsonValue;
        Array* JsonArray;
    };

    JsonVariable(VariableTypes type) {
        this->type = type;
    }
};

BLITZ3D(JsonVariable*) JsonParseFromFile(BBStr path) {
    std::ifstream file(path);
    std::string json, line;
    while (std::getline(file, line)) {
        json += line + '\n';
    }
    JsonVariable* object = new JsonVariable(VariableTypes::Document);
    object->JsonDocument = new Document();
    object->JsonDocument->Parse<kParseCommentsFlag>(json.c_str());
    if (object->JsonDocument->HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}", path));
    return object;
}

BLITZ3D(JsonVariable*) JsonParseFromString(BBStr json) {
    JsonVariable* object = new JsonVariable(VariableTypes::Document);
    object->JsonDocument = new Document();
    object->JsonDocument->Parse<kParseCommentsFlag>(json);
    if (object->JsonDocument->HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromString", std::format("Document parsing failed: {}", json));
    return object;
}

BLITZ3D(JsonVariable*) JsonGetValue(JsonVariable* object, BBStr name) {
    switch (object->type) {
    case VariableTypes::Document: {
        if (!object->JsonDocument->HasMember(name)) {
            BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in document!", name));
            return new JsonVariable(VariableTypes::Null);
        }
        JsonVariable* value = new JsonVariable(VariableTypes::Value);
        value->JsonValue = &(*object->JsonDocument)[name];
        return value;
    }
    case VariableTypes::Value: {
        if (!object->JsonValue->HasMember(name)) {
            BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in value!", name));
            return new JsonVariable(VariableTypes::Null);
        }
        JsonVariable* value = new JsonVariable(VariableTypes::Value);
        value->JsonValue = &(*object->JsonValue)[name];
        return value;
    }
    default: {
        BlitzToolbox::runtime_exception("JsonGetValue", "Invalid argument!");
        return new JsonVariable(VariableTypes::Null);
    }
    }
}

BLITZ3D(int) JsonIsString(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).IsString();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsString", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsInt(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).IsInt();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsInt", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsFloat(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).IsFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsFloat", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsBool(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).IsFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsBool", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsArray(JsonVariable* object) {
    switch (object->type) {
    case VariableTypes::Document: {
        return (*object->JsonDocument).IsArray();
    }
    case VariableTypes::Value: {
        return (*object->JsonValue).IsArray();
    }
    default: {
        BlitzToolbox::runtime_exception("JsonIsArray", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(int) JsonIsObject(JsonVariable* object) {
    switch (object->type) {
    case VariableTypes::Document: {
        return (*object->JsonDocument).IsObject();
    }
    case VariableTypes::Value: {
        return (*object->JsonValue).IsObject();
    }
    default: {
        BlitzToolbox::runtime_exception("JsonIsObject", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(int) JsonIsNull(JsonVariable* object) {
    switch (object->type) {
    case VariableTypes::Document: {
        return (*object->JsonDocument).IsNull();
    }
    case VariableTypes::Value: {
        return (*object->JsonValue).IsNull();
    }
    default: {
        BlitzToolbox::runtime_exception("JsonIsNull", "Invalid argument!");
        return true;
    }
    }
}

BLITZ3D(BBStr) JsonGetString(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).GetString();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetString", "Invalid argument!");
        return "";
    }
}

BLITZ3D(int) JsonGetInt(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).GetInt();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetInt", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(float) JsonGetFloat(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).GetFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetFloat", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(int) JsonGetBool(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return (*object->JsonValue).GetBool();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetBool", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonGetArray(JsonVariable* object) {
    switch (object->type) {
    case VariableTypes::Document: {
        JsonVariable* array = new JsonVariable(VariableTypes::Array);
        array->JsonArray = new Array((*object->JsonDocument).GetArray());
        return array;
    }
    case VariableTypes::Value: {
        JsonVariable* array = new JsonVariable(VariableTypes::Array);
        array->JsonArray = new Array((*object->JsonValue).GetArray());
        return array;
    }
    default: {
        BlitzToolbox::runtime_exception("JsonGetArray", "Invalid argument!");
        return new JsonVariable(VariableTypes::Null);
    }
    }
}

BLITZ3D(int) JsonGetArraySize(JsonVariable* object) {
    if (object->type == VariableTypes::Array) {
        return (*object->JsonArray).Size();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArraySize", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonGetArrayValue(JsonVariable* object, int index) {
    if (object->type == VariableTypes::Array) {
        if ((*object->JsonArray).Size() <= index) {
            BlitzToolbox::runtime_exception("JsonGetArrayValue", "Array index out of bounds!");
            return new JsonVariable(VariableTypes::Null);
        }
        else {
            JsonVariable* value = new JsonVariable(VariableTypes::Value);
            value->JsonValue = &(*object->JsonArray)[index];
            return value;
        }
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArrayValue", "Invalid argument!");
        return new JsonVariable(VariableTypes::Null);
    }
}

BLITZ3D(int) JsonGetArrayCapacity(JsonVariable* object) {
    if (object->type == VariableTypes::Array) {
        return (*object->JsonArray).Capacity();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArrayCapacity", "Invalid argument!");
        return 0;
    }
}