/*
* Blib - A part of BlitzToolbox
* Zip Utils for Blitz3D.
*
* v1.01 2023.11.5
*/

#include "../BlitzToolbox.hpp"
#include "unzip.h"
#include "zip.h"

BLITZ3D(HZIP) BlibCreateZip(BBStr file, BBStr password) {
	return CreateZip(file, password);
}

BLITZ3D(HZIP) BlibOpenZip(BBStr file, BBStr password) {
	return OpenZip(file, password);
}

BLITZ3D(void) BlibZipAdd(HZIP hzip, BBStr dst, BBStr src) {
	ZipAdd(hzip, dst, src);
}

BLITZ3D(void) BlibZipRemove(HZIP hzip, BBStr dst) {
	ZipAdd(hzip, dst, 0);
}

BLITZ3D(void) BlibZipAddFolder(HZIP hzip, BBStr dst) {
	ZipAddFolder(hzip, dst);
}

BLITZ3D(void) BlibCloseZip(HZIP hzip) {
	CloseZip(hzip);
}

BLITZ3D(ZIPENTRY*) BlibGetZipItem(HZIP hzip, int index) {
	ZIPENTRY* ze = new ZIPENTRY();
	GetZipItem(hzip, index, ze);
	return ze;
}

BLITZ3D(void) BlibUnzipItem(HZIP hzip, int index, ZIPENTRY* zipentry, BBStr dst) {
	SetUnzipBaseDir(hzip, dst);
	UnzipItem(hzip, index, zipentry->name);
}

BLITZ3D(int) BlibFindZipItem(HZIP hzip, BBStr name, bool ic, ZIPENTRY* zipentry) {
	int index;
	FindZipItem(hzip, name, ic, &index, zipentry);
	return index;
}

BLITZ3D(void) BlibSetUnzipBaseDir(HZIP hzip, BBStr dir) {
	SetUnzipBaseDir(hzip, dir);
}

BLITZ3D(BBStr) BlibZipEntryName(ZIPENTRY* zipentry) {
	return zipentry->name;
}

BLITZ3D(void) BlibUnzip(BBStr zip, BBStr dst, BBStr password) {
	HZIP hz = OpenZip(zip, password);
	SetUnzipBaseDir(hz, dst);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
		UnzipItem(hz, zi, ze.name);
	}
	CloseZip(hz);
}