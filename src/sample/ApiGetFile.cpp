#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetFile.h"

StkObject* ApiGetFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TargetFileName[FILENAME_MAX];
	wchar_t OffsetStr[16];
	StkStringParser::ParseInto2Params(UrlPath, L"/api/file/$/$/", L'$', TargetFileName, FILENAME_MAX, OffsetStr, 16);
	size_t Offset = StkPlWcsToL(OffsetStr);
	wchar_t TargetFullPath[FILENAME_MAX];
	GetFullPathFromFileName(TargetFullPath, TargetFileName);
	size_t FileSize = StkPlGetFileSize(TargetFullPath);
	if (FileSize < 0) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_FILE_NOTEXIST, MessageProc::GetMsgEng(MSG_FILE_NOTEXIST), MessageProc::GetMsgJpn(MSG_FILE_NOTEXIST));
		*ResultCode = 400;
		return TmpObj;
	}
	if (Offset > FileSize) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_FILE_EXCEED_SIZE, MessageProc::GetMsgEng(MSG_FILE_EXCEED_SIZE), MessageProc::GetMsgJpn(MSG_FILE_EXCEED_SIZE));
		*ResultCode = 400;
		return TmpObj;
	}

	char* Buffer = new char[1000000];
	wchar_t* HexBuf = new wchar_t[2000001];
	size_t ActSize;
	void* FilePtr = StkPlOpenFileForRead(TargetFullPath);
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
	delete Buffer;

	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjD = new StkObject(L"Data");
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObjD->AppendChildElement(new StkObject(L"FileData", HexBuf));
	TmpObj->AppendChildElement(TmpObjD);
	delete HexBuf;
	*ResultCode = 200;
	return TmpObj;
}
