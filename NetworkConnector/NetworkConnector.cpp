/*
* NetworkConnector - A part of BlitzToolbox
* Parse domain's TXT and download files.
*
* v1.02 2022.12.1
*/

#include "../BlitzToolbox.hpp"
#include <Windows.h>
#include <wininet.h>
#include <WinDNS.h>
#include <filesystem>

using std::string;

BLITZ3D(BBStr) ParseDomainTXT(BBStr txt, BBStr key) {
	string s1 = txt, s2 = key, result;
	int n, a;
	if ((n = s1.find(key)) != string::npos) result = s1.substr(n);
	if ((a = result.find(';')) != string::npos) result = result.substr(s2.length() + 1, a - s2.length() - 1);
	return BlitzToolbox::getCharPtr(result);
}

BLITZ3D(BBStr) GetDomainTXT(BBStr domain) {
	PDNS_RECORD pResult = NULL;
	DnsQuery_A(domain, DNS_TYPE_TEXT, DNS_QUERY_BYPASS_CACHE, NULL, &pResult, NULL);
	std::string record = std::string(pResult->Data.TXT.pStringArray[0]);
	DnsRecordListFree(pResult, DnsFreeRecordListDeep);
	return BlitzToolbox::getCharPtr(record);
}

BLITZ3D(bool) DownloadFile(BBStr url, BBStr file)
{
	/* https://blog.csdn.net/HW140701/article/details/78207490 */
	byte Temp[1024];
	ULONG Number = 1;

	FILE* stream;
	HINTERNET hSession = InternetOpen("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
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