/*
* NetworkConnector - A part of BlitzToolbox
* Parse domain's TXT and download files.
* 
* v1.0 2022.8.14
*/

#include "../BlitzToolbox.hpp"
#include <Windows.h>
#include <wininet.h>
#include <filesystem>
#include <format>

using std::string;

string exec(const char* cmd) {
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "";
    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) result = buffer;
    }
    _pclose(pipe);
    return result;
}

string clearTabLeft(string src) {
    unsigned int pos = 0;
    string result = src;
    for (;;) {
        if (src[pos] == '	') {
            result = src.substr(pos + 1);
            pos++;
        }
        else break;
    }
    return result;
}

BLITZ3D(BBStr) ParseDomainTXT(BBStr txt, BBStr key) {
    string s1 = txt, s2 = key, result;
    int n, a;
    if ((n = s1.find(key)) != string::npos) result = s1.substr(n);
    if ((a = result.find(';')) != string::npos) result = result.substr(s2.length() + 1, a - s2.length() - 1);
    return getCharPtr(result);
}

BLITZ3D(BBStr) GetDomainTXT(BBStr domain) {
    string result = clearTabLeft(exec(std::format("nslookup -qt=TXT {0}", domain).c_str()));
    if (result[0] == '\"') result = result.substr(1);
    if (result[result.length() - 2] == '\"')result = result.substr(0, result.length() - 2);
    if (result[result.length() - 1] != ';') result += ';';
    return getCharPtr(result);
}

BLITZ3D(bool) DownloadFile(BBStr url, BBStr file)
{
    /* https://blog.csdn.net/HW140701/article/details/78207490 */
    byte Temp[1024];
    ULONG Number = 1;

    FILE* stream;
    HINTERNET hSession = InternetOpen(L"RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession != NULL)
    {
        HINTERNET handle2 = InternetOpenUrl(hSession, (LPCWSTR)url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
        if (handle2 != NULL)
        {
            if ((stream = fopen(file, "wb")) != NULL)
            {
                while (Number > 0)
                {
                    InternetReadFile(handle2, Temp, 1024 - 1, &Number);
                    fwrite(Temp, sizeof(char), Number, stream);
                }
                fclose(stream);
            }
            InternetCloseHandle(handle2);
            handle2 = NULL;
        }
        InternetCloseHandle(hSession);
        hSession = NULL;
    }

    return std::filesystem::exists(file);
}