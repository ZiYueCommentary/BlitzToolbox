/*
* NetworkConnector - A part of BlitzToolbox
* Parse domain's TXT and download files.
* 
* v1.0 2022.5.2
*/

#include <math.h>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include <wininet.h> 
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "wininet.lib")
#pragma warning(disable : 4996)

#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall

typedef const char* BBStr;
using namespace std;

string exec(const char* cmd) {
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) return "0";
	char buffer[128];
	string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result = buffer;
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

BLITZ3D(BBStr) ParseDomainTXT(BBStr txt, BBStr name) {
	string s1 = txt;
	string s2 = name;
	string result;
	int n, a;
	if ((n = s1.find(name)) != string::npos) result = s1.substr(n);
	if ((a = result.find(';')) != string::npos) result = result.substr(s2.length() + 1, a - s2.length() - 1);
	return result.c_str();
}

BLITZ3D(BBStr) GetDomainTXT(BBStr domain) {
	string cmd = "nslookup -qt=TXT ";
	cmd += domain;
	string result = exec(cmd.c_str());
	result = clearTabLeft(result);
	if (result[0] == '\"') result = result.substr(1);
	if (result[result.length() - 2] == '\"')result = result.substr(0, result.length() - 2);
	if (result[result.length() - 1] != ';') result += ';';
	return result.c_str();
}

BLITZ3D(void) DownloadFile(BBStr url, BBStr file)
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
}