#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetFile.h"

ApiGetFile::ApiGetFile()
{
	for (int Loop = 0; Loop < 10; Loop++) {
		OrgDat[Loop] = new char[1000000];
		HexDat[Loop] = new wchar_t[2000001];
		LockTableOrgDat[Loop] = NULL;
		LockTableHexDat[Loop] = NULL;
	}
}

ApiGetFile::~ApiGetFile()
{
	for (int Loop = 0; Loop < 10; Loop++) {
		delete [] OrgDat[Loop];
		delete [] HexDat[Loop];
	}
}

char* ApiGetFile::GetOrgDat()
{
	StkPlLockCs(&CsGetFile);
	char* RetPtr = NULL;
	while (!RetPtr) {
		for (int Loop = 0; Loop < 10; Loop++) {
			if (LockTableOrgDat[Loop] == NULL) {
				LockTableOrgDat[Loop] = OrgDat[Loop];
				RetPtr = LockTableOrgDat[Loop];
			}
		}
		if (!RetPtr) {
			break;
		} else {
			StkPlSleepMs(50);
		}
	}
	StkPlUnlockCs(&CsGetFile);
	return RetPtr;
}

void ApiGetFile::ReleaseOrgDat(char* Target)
{
	StkPlLockCs(&CsGetFile);
	for (int Loop = 0; Loop < 10; Loop++) {
		if (LockTableOrgDat[Loop] == Target) {
			LockTableOrgDat[Loop] = NULL;
		}
	}
	StkPlUnlockCs(&CsGetFile);
}

wchar_t* ApiGetFile::GetHexDat()
{
	StkPlLockCs(&CsGetFile);
	wchar_t* RetPtr = NULL;
	while (!RetPtr) {
		for (int Loop = 0; Loop < 10; Loop++) {
			if (LockTableHexDat[Loop] == NULL) {
				LockTableHexDat[Loop] = HexDat[Loop];
				RetPtr = LockTableHexDat[Loop];
			}
		}
		if (!RetPtr) {
			break;
		} else {
			StkPlSleepMs(50);
		}
	}
	StkPlUnlockCs(&CsGetFile);
	return RetPtr;
}

void ApiGetFile::ReleaseHexDat(wchar_t* Target)
{
	StkPlLockCs(&CsGetFile);
	for (int Loop = 0; Loop < 10; Loop++) {
		if (LockTableHexDat[Loop] == Target) {
			LockTableHexDat[Loop] = NULL;
		}
	}
	StkPlUnlockCs(&CsGetFile);
}

StkObject* ApiGetFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TargetFileName[FILENAME_MAX * 4];
	wchar_t OffsetStr[16];
	StkStringParser::ParseInto2Params(UrlPath, L"/api/file/$/$/", L'$', TargetFileName, FILENAME_MAX, OffsetStr, 16);
	wchar_t TargetFileNameAct[FILENAME_MAX] = L"";
	DecodeURL(TargetFileName, FILENAME_MAX * 4, TargetFileNameAct, FILENAME_MAX);
	size_t Offset = StkPlWcsToL(OffsetStr);
	wchar_t TargetFullPath[FILENAME_MAX];
	GetFullPathFromFileName(TargetFullPath, TargetFileNameAct);

	size_t FileSize = StkPlGetFileSize(TargetFullPath);
	if (FileSize < 0) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_FILE_ACCESS_ERROR, MessageProc::GetMsgEng(MSG_FILE_ACCESS_ERROR), MessageProc::GetMsgJpn(MSG_FILE_ACCESS_ERROR));
		*ResultCode = 400;
		return TmpObj;
	}
	if (Offset > FileSize) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_FILE_EXCEED_SIZE, MessageProc::GetMsgEng(MSG_FILE_EXCEED_SIZE), MessageProc::GetMsgJpn(MSG_FILE_EXCEED_SIZE));
		*ResultCode = 400;
		return TmpObj;
	}

	void* FilePtr = StkPlOpenFileForRead(TargetFullPath);
	if (FilePtr == NULL) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_FILE_ACCESS_ERROR, MessageProc::GetMsgEng(MSG_FILE_ACCESS_ERROR), MessageProc::GetMsgJpn(MSG_FILE_ACCESS_ERROR));
		*ResultCode = 400;
		return TmpObj;
	}
	char* Buffer = GetOrgDat();
	wchar_t* HexBuf = GetHexDat();
	size_t ActSize;
	StkPlSeekFromBegin(FilePtr, Offset);
	StkPlRead(FilePtr, Buffer, 1000000, &ActSize);
	StkPlCloseFile(FilePtr);
	wchar_t HexChar[16] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
	size_t Loop = 0;
	for (; Loop < ActSize; Loop++) {
		HexBuf[Loop * 2] = HexChar[(unsigned char)Buffer[Loop] / 16];
		HexBuf[Loop * 2 + 1] = HexChar[(unsigned char)Buffer[Loop] % 16];
	}
	HexBuf[Loop * 2] = '\0';
	ReleaseOrgDat(Buffer);

	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjD = new StkObject(L"Data");
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObjD->AppendChildElement(new StkObject(L"FileName", TargetFileNameAct));
	TmpObjD->AppendChildElement(new StkObject(L"FileSize", (int)FileSize));
	TmpObjD->AppendChildElement(new StkObject(L"FileOffset", (int)Offset));
	TmpObjD->AppendChildElement(new StkObject(L"FileData", HexBuf));
	TmpObj->AppendChildElement(TmpObjD);
	ReleaseHexDat(HexBuf);
	*ResultCode = 200;
	return TmpObj;
}
