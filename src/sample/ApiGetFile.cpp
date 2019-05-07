#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "ApiGetFile.h"

StkObject* ApiGetFile::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t TargetFileName[FILENAME_MAX];
	wchar_t OffsetStr[16];
	size_t Offset = StkPlWcsToL(OffsetStr);
	StkStringParser::ParseInto2Params(UrlPath, L"/api/file/$/$/", L'$', TargetFileName, FILENAME_MAX, OffsetStr, 16);
	size_t FileSize = StkPlGetFileSize(TargetFileName);
	if (FileSize < 0) {
		StkObject* TmpObj = new StkObject(L"");
		TmpObj->AppendChildElement(new StkObject(L"Msg0", L"Target File does not exist."));
		*ResultCode = 400;
		return TmpObj;
	}
	if (Offset > FileSize) {
		StkObject* TmpObj = new StkObject(L"");
		TmpObj->AppendChildElement(new StkObject(L"Msg0", L"The specified offset exceeds the file size."));
		*ResultCode = 400;
		return TmpObj;
	}

	char Buffer[1000000];
	wchar_t HexBuf[2000000];
	size_t ActSize;
	void* FilePtr = StkPlOpenFileForRead(TargetFileName);
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

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"FileSize", (int)ActSize));
	TmpObj->AppendChildElement(new StkObject(L"FileData", HexBuf));
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
