/*
* BlitzToolbox - A part of BlitzToolbox
* Custom library for scpcb-ue.
*
* v1.0 2023.9.9
*/

#include "../BlitzToolbox.hpp"
#include <Windows.h>
#include <filesystem>
#include <wininet.h>

BLITZ3D(BBStr) FindNextDirectory(BBStr path, BBStr directory, BBStr def) {
    WIN32_FIND_DATAA fdFindData;
    HANDLE hFind;
    std::string filename = path;
    filename += "\\*.*";
    int count = 0;
    BOOL done;
    BOOL nextfolder = !std::filesystem::exists(std::filesystem::path(std::string(path) + "\\" + directory));
    hFind = FindFirstFileA(filename.c_str(), &fdFindData);
    done = hFind != INVALID_HANDLE_VALUE;
    while (done)
    {
        if (strcmp(fdFindData.cFileName, ".") && strcmp(fdFindData.cFileName, ".."))
        {
            filename = path;
            filename += "\\";
            filename += fdFindData.cFileName;
            if ((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
                if (nextfolder) return getCharPtr(BlitzToolbox::replace_all(filename, std::string(path) + "\\", ""));
                if (filename == std::string(path) + "\\" + directory) nextfolder = TRUE;
            }
        }
        done = FindNextFileA(hFind, &fdFindData);
    }
    FindClose(hFind);
    return def;
}

BLITZ3D(void) DownloadFileThread(BBStr url, BBStr file) {
    BBStr cUrl = getCharPtr(url), cFile = getCharPtr(file);
    std::thread([cUrl, cFile]() {
        byte Temp[1024];
        ULONG Number = 1;

        FILE* stream;
        HINTERNET hSession = InternetOpen("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hSession != NULL)
        {
            HINTERNET handle2 = InternetOpenUrl(hSession, cUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
            if (handle2 != NULL)
            {
                while (Number > 0)
                {
                    fopen_s(&stream, cFile, "ab+");
                    InternetReadFile(handle2, Temp, 1024 - 1, &Number);
                    fwrite(Temp, sizeof(char), Number, stream);
                    fclose(stream);
                }
                InternetCloseHandle(handle2);
                handle2 = NULL;
            }
            InternetCloseHandle(hSession);
            hSession = NULL;
        }
        }).detach();
}
