/*
* RapidBson - A part of BlitzToolbox
* A fast JSON parser/generator for Blitz3D with both SAX/DOM style API.
*
* v1.0 2024.9.17
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
typedef void* Object;

struct DocumentObj {
    BBStr identifier = DOCUMENT_IDENT;
    Document document;
};

BLITZ3D(DocumentObj*) JsonParseFromFile(BBStr path) {
    std::ifstream file(path);
    std::string json, line;
    while (std::getline(file, line)) {
        json += line + '\n';
    }
    DocumentObj* obj = new DocumentObj();
    obj->document.Parse<kParseCommentsFlag>(json.c_str());
    if (obj->document.HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}!", path));
    return obj;
}

BLITZ3D(DocumentObj*) JsonParseFromString(BBStr json) {
    DocumentObj* obj = new DocumentObj();
    obj->document.Parse<kParseCommentsFlag>(json);
    if (obj->document.HasParseError()) BlitzToolbox::runtime_exception("JsonParseFromFile", std::format("Document parsing failed: {}", json));
    return obj;
}

BLITZ3D(Value*) JsonGetValue(Object obj, BBStr name) {
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        if (!doc->document.HasMember(name)) BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in document!", name));
        return &doc->document[name];
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        if (!val->HasMember(name)) BlitzToolbox::runtime_exception("JsonGetValue", std::format("No value named \"{}\" in value!", name));
        return &(*val)[name];
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

BLITZ3D(int) JsonIsBool(Value* value) {
    return value->IsBool();
}

BLITZ3D(int) JsonIsArray(Object* obj) {
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        return doc->document.IsArray();
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        return val->IsArray();
    }
}

BLITZ3D(int) JsonIsObject(Object* obj) {
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        return doc->document.IsObject();
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        return val->IsObject();
    }
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
    if (!value->IsString()) BlitzToolbox::runtime_exception("JsonGetString", "Value is not a string!");
    return value->GetString();
}

BLITZ3D(int) JsonGetInt(Value* value) {
    if (!value->IsInt()) BlitzToolbox::runtime_exception("JsonGetInt", "Value is not an integer!");
    return value->GetInt();
}

BLITZ3D(float) JsonGetFloat(Value* value) {
    if (!value->IsFloat()) BlitzToolbox::runtime_exception("JsonGetFloat", "Value is not a float!");
    return value->GetFloat();
}

BLITZ3D(int) JsonGetBool(Value* value) {
    if (!value->IsBool()) BlitzToolbox::runtime_exception("JsonGetBool", "Value is not a boolean!");
    return value->GetBool();
}

BLITZ3D(Array*) JsonGetArray(Object* obj) {
    DocumentObj* doc = reinterpret_cast<DocumentObj*>(obj);
    if (doc->identifier == DOCUMENT_IDENT) { // obj is a document
        if (!doc->document.IsArray()) BlitzToolbox::runtime_exception("JsonGetArray", "Document is not an array!");
        return new Array(doc->document.GetArray());
    }
    else { // obj is a value
        Value* val = reinterpret_cast<Value*>(obj);
        if (!val->IsArray()) BlitzToolbox::runtime_exception("JsonGetArray", "Value is not an array!");
        return new Array(val->GetArray());
    }
}

BLITZ3D(int) JsonGetArraySize(Array* array) {
    return array->Size();
}

BLITZ3D(Value*) JsonGetArrayValue(Array* array, int index) {
    return &(*array)[index];
}

BLITZ3D(int) JsonGetArrayCapacity(Array* array) {
    return array->Capacity();
}