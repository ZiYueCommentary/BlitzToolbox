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

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include "../BlitzToolbox.hpp"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::clog << "RapidBson: In the development stage. May not be stable or useable.\n";
    }
    return TRUE;
}

using namespace rapidjson;
using Array = GenericValue<UTF8<>>::Array;

enum class VariableType
{
    Document, Value, Array, Writer, Null
};

struct __Writer {
    StringBuffer* buffer;
    Writer<StringBuffer>* writer;

    __Writer() {
        this->buffer = new StringBuffer();
        this->writer = new Writer(*buffer);
    }

    ~__Writer() {
        delete buffer;
        delete writer;
    }
};

struct JsonVariable {
    VariableType type;
    union {
        Document* JsonDocument = nullptr;
        Value* JsonValue;
        Array* JsonArray;
        __Writer* JsonWriter;
    };

    JsonVariable(VariableType type) {
        this->type = type;
    }
};

enum __SuppressWarning {
    kNoSuppressedWarnings,
    kNoRuntimeExceptions,
    kNoExceptionLogs
};

static int _SuppressedWarnings = kNoSuppressedWarnings;

constexpr void __rapidbson_runtime_exception(BBStr function, const std::string& message) {
    if (!(_SuppressedWarnings & kNoRuntimeExceptions)) {
        BlitzToolbox::runtime_exception(function, message);
    }
    if (!(_SuppressedWarnings & kNoExceptionLogs)) {
        std::cerr << std::format("{}: {}\n", function, message);
    }
}

BLITZ3D(void) JsonSuppressWarnings(int flags) {
    _SuppressedWarnings = flags;
}

BLITZ3D(JsonVariable*) JsonParseFromFile(BBStr path) {
    std::ifstream file(path);
    std::string json, line;
    while (std::getline(file, line)) {
        json += line + '\n';
    }
    JsonVariable* object = new JsonVariable(VariableType::Document);
    object->JsonDocument = new Document();
    object->JsonDocument->Parse<kParseCommentsFlag>(json.c_str());
    if (object->JsonDocument->HasParseError()) {
        __rapidbson_runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}", path));
    }
    return object;
}

BLITZ3D(JsonVariable*) JsonParseFromString(BBStr json) {
    JsonVariable* object = new JsonVariable(VariableType::Document);
    object->JsonDocument = new Document();
    object->JsonDocument->Parse<kParseCommentsFlag>(json);
    if (object->JsonDocument->HasParseError()) {
        __rapidbson_runtime_exception("JsonParseFromString", std::format("Document parsing failed: {}", json));
    }
    return object;
}

BLITZ3D(int) JsonHasParseError(JsonVariable* document) {
    if (document->type == VariableType::Document) {
        return document->JsonDocument->HasParseError();
    }
    else {
        __rapidbson_runtime_exception("JsonHasParseError", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonGetParseErrorCode(JsonVariable* document) {
    if (document->type == VariableType::Document) {
        return document->JsonDocument->GetParseError();
    }
    else {
        __rapidbson_runtime_exception("JsonGetParseErrorCode", "Invalid argument!");
        return -1;
    }
}

BLITZ3D(JsonVariable*) JsonGetValue(JsonVariable* object, BBStr name) {
    switch (object->type) {
    case VariableType::Document: {
        if (!object->JsonDocument->HasMember(name)) {
            __rapidbson_runtime_exception("JsonGetValue", std::format("No value named \"{}\" in document!", name));
            return new JsonVariable(VariableType::Null);
        }
        JsonVariable* value = new JsonVariable(VariableType::Value);
        value->JsonValue = &(*object->JsonDocument)[name];
        return value;
    }
    case VariableType::Value: {
        if (!object->JsonValue->HasMember(name)) {
            __rapidbson_runtime_exception("JsonGetValue", std::format("No value named \"{}\" in value!", name));
            return new JsonVariable(VariableType::Null);
        }
        JsonVariable* value = new JsonVariable(VariableType::Value);
        value->JsonValue = &(*object->JsonValue)[name];
        return value;
    }
    default: {
        __rapidbson_runtime_exception("JsonGetValue", "Invalid argument!");
        return new JsonVariable(VariableType::Null);
    }
    }
}

BLITZ3D(int) JsonHasMember(JsonVariable* object, BBStr name) {
    switch (object->type) {
    case VariableType::Document: {
        return object->JsonDocument->HasMember(name);
    }
    case VariableType::Value: {
        return object->JsonValue->HasMember(name);
    }
    default: {
        __rapidbson_runtime_exception("JsonHasMember", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(int) JsonIsString(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        return object->JsonValue->IsString();
    }
    else {
        __rapidbson_runtime_exception("JsonIsString", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsInt(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        return object->JsonValue->IsInt();
    }
    else {
        __rapidbson_runtime_exception("JsonIsInt", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsFloat(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        return object->JsonValue->IsFloat();
    }
    else {
        __rapidbson_runtime_exception("JsonIsFloat", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsBool(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        return object->JsonValue->IsFloat();
    }
    else {
        __rapidbson_runtime_exception("JsonIsBool", "Invalid argument!");
        return false;
    }
}

BLITZ3D(int) JsonIsArray(JsonVariable* object) {
    switch (object->type) {
    case VariableType::Document: {
        return object->JsonDocument->IsArray();
    }
    case VariableType::Value: {
        return object->JsonValue->IsArray();
    }
    default: {
        __rapidbson_runtime_exception("JsonIsArray", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(int) JsonIsObject(JsonVariable* object) {
    switch (object->type) {
    case VariableType::Document: {
        return object->JsonDocument->IsObject();
    }
    case VariableType::Value: {
        return object->JsonValue->IsObject();
    }
    default: {
        __rapidbson_runtime_exception("JsonIsObject", "Invalid argument!");
        return false;
    }
    }
}

BLITZ3D(int) JsonIsNull(JsonVariable* object) {
    switch (object->type) {
    case VariableType::Document: {
        return object->JsonDocument->IsNull();
    }
    case VariableType::Value: {
        return object->JsonValue->IsNull();
    }
    case VariableType::Null: {
        return true;
    }
    default: {
        __rapidbson_runtime_exception("JsonIsNull", "Invalid argument!");
        return true;
    }
    }
}

BLITZ3D(BBStr) JsonGetString(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        if (!object->JsonValue->IsString()) {
            __rapidbson_runtime_exception("JsonGetString", "Not a string value!");
            return "";
        }
        else {
            return object->JsonValue->GetString();
        }
    }
    else {
        __rapidbson_runtime_exception("JsonGetString", "Invalid argument!");
        return "";
    }
}

BLITZ3D(int) JsonGetInt(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        if (!object->JsonValue->IsInt()) {
            __rapidbson_runtime_exception("JsonGetInt", "Not an integer value!");
            return 0;
        }
        else {
            return object->JsonValue->GetInt();
        }
    }
    else {
        __rapidbson_runtime_exception("JsonGetInt", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(float) JsonGetFloat(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        if (!object->JsonValue->IsFloat()) {
            __rapidbson_runtime_exception("JsonGetFloat", "Not a float value!");
            return 0;
        }
        else {
            return object->JsonValue->GetFloat();
        }
    }
    else {
        __rapidbson_runtime_exception("JsonGetFloat", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(int) JsonGetBool(JsonVariable* object) {
    if (object->type == VariableType::Value) {
        if (!object->JsonValue->IsBool()) {
            __rapidbson_runtime_exception("JsonGetBool", "Not a boolean value!");
            return 0;
        }
        else {
            return object->JsonValue->GetBool();
        }
    }
    else {
        __rapidbson_runtime_exception("JsonGetBool", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonGetArray(JsonVariable* object) {
    switch (object->type) {
    case VariableType::Document: {
        if (!object->JsonDocument->IsArray()) {
            __rapidbson_runtime_exception("JsonGetArray", "Not an array document!");
            return new JsonVariable(VariableType::Null);
        }
        JsonVariable* array = new JsonVariable(VariableType::Array);
        array->JsonArray = new Array(object->JsonDocument->GetArray());
        return array;
    }
    case VariableType::Value: {
        if (!object->JsonValue->IsArray()) {
            __rapidbson_runtime_exception("JsonGetArray", "Not an array value!");
            return new JsonVariable(VariableType::Null);
        }
        JsonVariable* array = new JsonVariable(VariableType::Array);
        array->JsonArray = new Array(object->JsonValue->GetArray());
        return array;
    }
    default: {
        __rapidbson_runtime_exception("JsonGetArray", "Invalid argument!");
        return new JsonVariable(VariableType::Null);
    }
    }
}

BLITZ3D(int) JsonGetArraySize(JsonVariable* object) {
    if (object->type == VariableType::Array) {
        return object->JsonArray->Size();
    }
    else {
        __rapidbson_runtime_exception("JsonGetArraySize", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonGetArrayValue(JsonVariable* object, int index) {
    if (object->type == VariableType::Array) {
        if (object->JsonArray->Size() <= index) {
            __rapidbson_runtime_exception("JsonGetArrayValue", "Array index out of bounds!");
            return new JsonVariable(VariableType::Null);
        }
        else {
            JsonVariable* value = new JsonVariable(VariableType::Value);
            value->JsonValue = &(*object->JsonArray)[index];
            return value;
        }
    }
    else {
        __rapidbson_runtime_exception("JsonGetArrayValue", "Invalid argument!");
        return new JsonVariable(VariableType::Null);
    }
}

BLITZ3D(int) JsonGetArrayCapacity(JsonVariable* object) {
    if (object->type == VariableType::Array) {
        return object->JsonArray->Capacity();
    }
    else {
        __rapidbson_runtime_exception("JsonGetArrayCapacity", "Invalid argument!");
        return 0;
    }
}

BLITZ3D(JsonVariable*) JsonCreateWriter() {
    JsonVariable* jsonVariable = new JsonVariable(VariableType::Writer);
    jsonVariable->JsonWriter = new __Writer();
    return jsonVariable;
}

BLITZ3D(JsonVariable*) JsonGetNewWriter(JsonVariable* object) {
    switch (object->type) {
    case VariableType::Document: {
        __Writer* writer = new __Writer();
        object->JsonDocument->Accept(*writer->writer);
        JsonVariable* jsonVariable = new JsonVariable(VariableType::Writer);
        jsonVariable->JsonWriter = writer;
        return jsonVariable;
    }
    case VariableType::Value: {
        __Writer* writer = new __Writer();
        object->JsonValue->Accept(*writer->writer);
        JsonVariable* jsonVariable = new JsonVariable(VariableType::Writer);
        jsonVariable->JsonWriter = writer;
        return jsonVariable;
    }
    default: {
        __rapidbson_runtime_exception("JsonGetNewWriter", "Invalid argument!");
        return new JsonVariable(VariableType::Null);
    }
    }
}

BLITZ3D(void) JsonDestroyWriter(JsonVariable* object) {
    if (object->type == VariableType::Writer) {
        delete object->JsonWriter;
        object->JsonWriter = nullptr;
    }
    else {
        __rapidbson_runtime_exception("JsonDestroyWriter", "Invalid argument!");
    }
}

BLITZ3D(BBStr) JsonGetWriterString(JsonVariable* object) {
    if (object->type == VariableType::Writer) {
        if (object->JsonWriter == nullptr) {
            __rapidbson_runtime_exception("JsonGetWriterString", "Writer is destroyed!");
            return "";
        }
        return object->JsonWriter->buffer->GetString();
    }
    else {
        __rapidbson_runtime_exception("JsonGetWriterString", "Invalid argument!");
    }
}

BLITZ3D(int) JsonGetWriterStringLength(JsonVariable* object) {
    if (object->type == VariableType::Writer) {
        if (object->JsonWriter == nullptr) {
            __rapidbson_runtime_exception("JsonGetWriterStringLength", "Writer is destroyed!");
            return 0;
        }
        return object->JsonWriter->buffer->GetLength();
    }
    else {
        __rapidbson_runtime_exception("JsonGetWriterStringLength", "Invalid argument!");
    }
}

BLITZ3D(BBStr) JsonGetFormattedWriterString(JsonVariable* object) {
    if (object->type == VariableType::Writer) {
        if (object->JsonWriter == nullptr) {
            __rapidbson_runtime_exception("JsonGetFormattedWriterString", "Writer is destroyed!");
            return "";
        }
        Document document;
        document.Parse(object->JsonWriter->buffer->GetString());
        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        document.Accept(writer);
        return BlitzToolbox::getCharPtr(buffer.GetString());
    }
    else {
        __rapidbson_runtime_exception("JsonGetFormattedJsonFromWriter", "Invalid argument!");
    }
}