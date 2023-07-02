/*
* RapidBson - A part of BlitzToolbox
* A fast JSON parser/generator for Blitz3D with both SAX/DOM style API.
*
* v1.0 2023.6.22
*/

///////////////////////////
/// IN DEVELOPING STAGE ///
///////////////////////////

#define DOCUMENT_IDENT "_RapidBson@DocObj"
#define SAFE_NULL(value, type) (value)->Is##type() ? (value)->Get##type() : NULL
#define PURE_NULL(value) (value)->IsNull() ? NULL : (value)

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include "../BlitzToolbox.hpp"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        printf_s("RapidBson: In the development stage. May not be stable or useable. \n");
    }
    return TRUE;
}

using namespace rapidjson;
using Array = GenericValue<UTF8<>>::Array;
typedef void* Object;

struct DocumentObj {
    BBStr identifier = DOCUMENT_IDENT;
    Document document;
    StringBuffer* prettyBuffer;
    PrettyWriter<StringBuffer>* prettyWriter;

    bool IsNull() {
        return document.IsNull();
    }

    DocumentObj() {
        prettyBuffer = new StringBuffer();
        prettyWriter = new PrettyWriter<StringBuffer>(*prettyBuffer);
    }

    ~DocumentObj() {
        delete prettyBuffer, prettyWriter;
    }
};

BLITZ3D(DocumentObj*) JsonParseFromFile(BBStr path) {
    std::ifstream file(path);
    std::string json, line;
    while (std::getline(file, line)) {
        json += line + '\n';
    }
    DocumentObj* obj = new DocumentObj();
    obj->document.Parse<kParseCommentsFlag>(json.c_str());
    return PURE_NULL(obj);
}

BLITZ3D(DocumentObj*) JsonParseFromString(BBStr json) {
    DocumentObj* obj = new DocumentObj();
    obj->document.Parse<kParseCommentsFlag>(json);
    return PURE_NULL(obj);
}

BLITZ3D(Value*) JsonGetValue(Object obj, BBStr name) {
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        return PURE_NULL(&doc->document[name]);
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        return PURE_NULL(&(*val)[name]);
    }
}

BLITZ3D(int) JsonIsString(Value* value) {
    return value->IsString();
}

BLITZ3D(int) JsonIsInt(Value* value) {
    return value->IsInt();
}

BLITZ3D(int) JsonIsFloat(Value* value) {
    return value->IsFloat();
}

BLITZ3D(int) JsonIsBoolean(Value* value) {
    return value->IsBool();
}

BLITZ3D(int) JsonIsArray(Value* value) {
    return value->IsArray();
}

BLITZ3D(int) JsonIsObject(Value* value) {
    return value->IsObject();
}

BLITZ3D(int) JsonIsNull(Object obj) {
    if (obj == nullptr) return true;
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        return doc->document.IsNull();
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        return val->IsNull();
    }
}

BLITZ3D(BBStr) JsonGetString(Value* value) {
    return SAFE_NULL(value, String);
}

BLITZ3D(int) JsonGetInt(Value* value) {
    return SAFE_NULL(value, Int);
}

BLITZ3D(float) JsonGetFloat(Value* value) {
    return SAFE_NULL(value, Float);
}

BLITZ3D(int) JsonGetBoolean(Value* value) {
    return SAFE_NULL(value, Bool);
}

BLITZ3D(Array*) JsonGetArray(Value* value) {
    return value->IsArray() ? new Array(value->GetArray()) : nullptr;
}

BLITZ3D(int) JsonGetArraySize(Array* array) {
    return array->Size();
}

BLITZ3D(Value*) JsonGetArrayValue(Array* array, int index) {
    return PURE_NULL(&(*array)[index]);
}

BLITZ3D(int) JsonGetArrayCapacity(Array* array) {
    return array->Capacity();
}