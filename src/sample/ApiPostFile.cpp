#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "ApiPostFile.h"

StkObject* ApiPostFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	if (ReqObj != NULL) {
		*ResultCode = 200;
		int Offset = -1;
		wchar_t* FileData = NULL;
		wchar_t* FileName = NULL;

		StkObject* CurObj = ReqObj->GetFirstChildElement();
		while (CurObj) {
			if (StkPlWcsCmp(CurObj->GetName(), L"FileOffset") == 0) {
				Offset = CurObj->GetIntValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"FileData") == 0) {
				FileData = CurObj->GetStringValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"FileName") == 0) {
				FileName = CurObj->GetStringValue();
			}
			CurObj = CurObj->GetNext();
		}
		if (Offset == -1 || FileData == NULL || FileName == NULL) {
			return NULL;
		}
		wchar_t* FileDataPtr = FileData;
		int FileDataLen = StkPlWcsLen(FileData);
		unsigned char* DataChar = new unsigned char[1000001];
		int DataCharIndex = 0;
		wchar_t HexNum[128];
		HexNum[L'0'] = 0;
		HexNum[L'1'] = 1;
		HexNum[L'2'] = 2;
		HexNum[L'3'] = 3;
		HexNum[L'4'] = 4;
		HexNum[L'5'] = 5;
		HexNum[L'6'] = 6;
		HexNum[L'7'] = 7;
		HexNum[L'8'] = 8;
		HexNum[L'9'] = 9;
		HexNum[L'A'] = 10;
		HexNum[L'B'] = 11;
		HexNum[L'C'] = 12;
		HexNum[L'D'] = 13;
		HexNum[L'E'] = 14;
		HexNum[L'F'] = 15;
		while (*FileDataPtr != L'\0') {
			if (DataCharIndex >= 1000000) {
				break;
			}
			DataChar[DataCharIndex] = HexNum[*FileDataPtr] * 16 + HexNum[*(FileDataPtr + 1)];
			DataCharIndex++;
			FileDataPtr += 2;
		}
		DataChar[DataCharIndex] = '\0';
		void* FilePtr = NULL;
		wchar_t TargetFullPath[DA_MAXLEN_OF_SERVERFILENAME];
		DataAccess::GetInstance()->GetFullPathFromFileName(TargetFullPath, FileName);
		if (Offset == 0) {
			FilePtr = StkPlOpenFileForWrite(TargetFullPath);
		} else {
			FilePtr = StkPlOpenFileForWrite(TargetFullPath, true);
		}
		StkPlSeekFromEnd(FilePtr, 0);
		size_t ActSize = 0;
		StkPlWrite(FilePtr, (char*)DataChar, DataCharIndex, &ActSize);
		StkPlCloseFile(FilePtr);
		delete DataChar;
	}
	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
