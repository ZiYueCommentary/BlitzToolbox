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
typedef void* Object;

enum class VariableTypes
{
    Document, Value, Array
};

struct JsonVariable {
    VariableTypes type;
    union {
        Document document = nullptr;
        Value value;
        GenericValue<UTF8<>>::Array array;
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
    object->document = Document();
    object->document.Parse<kParseCommentsFlag>(json.c_str());
    if (object->document.HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}", path));
    return object;
}

BLITZ3D(JsonVariable*) JsonParseFromString(BBStr json) {
    JsonVariable* object = new JsonVariable(VariableTypes::Document);
    object->document.Parse<kParseCommentsFlag>(json);
    if (object->document.HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}", json));
    return object;
}

BLITZ3D(JsonVariable*) JsonGetValue(JsonVariable* object, BBStr name) {
    switch (object->type) {
    case VariableTypes::Document: {
        if (!object->document.HasMember(name)) {
            BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in document!", name));
            return nullptr;
        }
        JsonVariable* value = new JsonVariable(VariableTypes::Value);
        value->value = object->document[name];
        return value;
    }
    case VariableTypes::Value: {
        if (!object->value.HasMember(name)) {
            BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in value!", name));
            return nullptr;
        }
        JsonVariable* value = new JsonVariable(VariableTypes::Value);
        value->value = object->value[name];
        return value;
    }
    default: {
        BlitzToolbox::runtime_exception("JsonGetValue", "Invalid argument!");
        return nullptr;
    }
    }
}

BLITZ3D(int) JsonIsString(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.IsString();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsString", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsInt(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.IsInt();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsInt", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsFloat(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.IsFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsFloat", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsBool(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.IsFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonIsBool", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsArray(JsonVariable* object) {
    switch (object->type) {
    case VariableTypes::Document: {
        return object->document.IsArray();
    }
    case VariableTypes::Value: {
        return object->value.IsArray();
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
        return object->document.IsObject();
    }
    case VariableTypes::Value: {
        return object->value.IsObject();
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
        return object->document.IsNull();
    }
    case VariableTypes::Value: {
        return object->value.IsNull();
    }
    default: {
        BlitzToolbox::runtime_exception("JsonIsNull", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(BBStr) JsonGetString(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.GetString();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetString", "Invalid argument!");
        return "";
    }
}

BLITZ3D(int) JsonGetInt(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.GetInt();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetInt", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(float) JsonGetFloat(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.GetFloat();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetFloat", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(int) JsonGetBool(JsonVariable* object) {
    if (object->type == VariableTypes::Value) {
        return object->value.GetBool();
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
        array->array = object->document.GetArray();
        return array;
    }
    case VariableTypes::Value: {
        JsonVariable* array = new JsonVariable(VariableTypes::Array);
        array->array = object->value.GetArray();
        return array;
    }
    default: {
        BlitzToolbox::runtime_exception("JsonGetArray", "Invalid argument!");
        return nullptr;
    }
    }
}

BLITZ3D(int) JsonGetArraySize(JsonVariable* object) {
    if (object->type == VariableTypes::Array) {
        return object->array.Size();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArraySize", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonGetArrayValue(JsonVariable* object, int index) {
    if (object->type == VariableTypes::Array) {
        if (object->array.Size() >= index) {
            BlitzToolbox::runtime_exception("JsonGetArrayValue", "Array index out of bounds!");
            return nullptr;
        }
        else {
            JsonVariable* value = new JsonVariable(VariableTypes::Value);
            value->value = object->array[index];
            return value;
        }
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArrayValue", "Invalid argument!");
        return nullptr;
    }
}

BLITZ3D(int) JsonGetArrayCapacity(JsonVariable* object) {
    if (object->type == VariableTypes::Array) {
        return object->array.Capacity();
    }
    else {
        BlitzToolbox::runtime_exception("JsonGetArrayCapacity", "Invalid argument!");
        return 0;
    }
}