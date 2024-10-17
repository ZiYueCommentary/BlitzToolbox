; RapidBson - A part of BlitzToolbox
; A fast JSON parser/generator for Blitz3D with both SAX/DOM style API.
; v1.0 2024.10.3
; https://github.com/ZiYueCommentary/BlitzToolbox

Const JsonNoSuppresWarnings% = 0
Const JsonNoRuntimeExceptions% = 1
Const JsonNoExceptionLogs% = 2

Const JsonParseErrorInvalidDocumentHandle% = -1
Const JsonParseErrorNone% = 0
Const JsonParseErrorDocumentEmpty% = 1
Const JsonParseErrorDocumentRootNotSingular% = 2
Const JsonParseErrorValueInvalid% = 3
Const JsonParseErrorObjectMissName% = 4
Const JsonParseErrorObjectMissColon% = 5
Const JsonParseErrorObjectMissCommaOrCurlyBracket% = 6
Const JsonParseErrorArrayMissCommaOrSquareBracket% = 7
Const JsonParseErrorStringUnicodeEscapeInvalidHex% = 8
Const JsonParseErrorStringUnicodeSurrogateInvalid% = 9
Const JsonParseErrorStringEscapeInvalid% = 10
Const JsonParseErrorStringMissQuotationMark% = 11
Const JsonParseErrorStringInvalidEncoding% = 12
Const JsonParseErrorNumberTooBig% = 13
Const JsonParseErrorNumberMissFraction% = 14
Const JsonParseErrorNumberMissExponent% = 15
Const JsonParseErrorTermination% = 16
Const JsonParseErrorUnspecificSyntaxError% = 17

Function JsonParseFromStream%(stream%)
    Local offset% = FilePos(stream)
    Local json$ = ""
    SeekFile(stream, 0)
    While Not Eof(stream)
        json = json + ReadLine(stream) + Chr(10)
    Wend
    SeekFile(stream, offset)
    Return JsonParseFromString(json)
End Function

Function JsonGetParseErrorMessage$(code%)
    Select code
        Case JsonParseErrorInvalidDocumentHandle
            Return "Invalid JSON document handle."
        Case JsonParseErrorNone
            Return "No error."
        Case JsonParseErrorDocumentEmpty
            Return "The document is empty."
        Case JsonParseErrorDocumentRootNotSingular
            Return "The document root must not be followed by other values."
        Case JsonParseErrorValueInvalid
            Return "Invalid value."
        Case JsonParseErrorObjectMissName
            Return "Missing a name for object member."
        Case JsonParseErrorObjectMissColon
            Return "Missing a colon after a name of object member."
        Case JsonParseErrorObjectMissCommaOrCurlyBracket
            Return "Missing a comma or '}' after an object member."
        Case JsonParseErrorArrayMissCommaOrSquareBracket
            Return "Missing a comma or ']' after an array element."
        Case JsonParseErrorStringUnicodeEscapeInvalidHex
            Return "Incorrect hex digit after \u escape in string."
        Case JsonParseErrorStringUnicodeSurrogateInvalid
            Return "The surrogate pair in string is invalid."
        Case JsonParseErrorStringEscapeInvalid
            Return "Invalid escape character in string."
        Case JsonParseErrorStringMissQuotationMark
            Return "Missing a closing quotation mark in string."
        Case JsonParseErrorStringInvalidEncoding
            Return "Invalid encoding in string."
        Case JsonParseErrorNumberTooBig
            Return "Number too big to be stored in double."
        Case JsonParseErrorNumberMissFraction
            Return "Miss fraction part in number."
        Case JsonParseErrorNumberMissExponent
            Return "Miss exponent in number."
        Case JsonParseErrorTermination
            Return "Terminate parsing due to Handler error."
        Case JsonParseErrorUnspecificSyntaxError
            Return "Unspecific syntax error."
        Default
            Return "Unknown error."
    End Select
End Function