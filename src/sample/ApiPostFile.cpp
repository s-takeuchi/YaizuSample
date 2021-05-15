#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiPostFile.h"

StkObject* ApiPostFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		UserId = -1;
	}
		
	if (ReqObj != NULL) {
		*ResultCode = 200;
		int Offset = -1;
		int Size = -1;
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
			if (StkPlWcsCmp(CurObj->GetName(), L"FileSize") == 0) {
				Size = CurObj->GetIntValue();
			}
			CurObj = CurObj->GetNext();
		}
		if (Offset == -1 || FileData == NULL || FileName == NULL) {
			return NULL;
		}
		wchar_t* FileDataPtr = FileData;
		int FileDataLen = (int)StkPlWcsLen(FileData);
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
		wchar_t TargetFullPath[FILENAME_MAX];
		GetFullPathFromFileName(TargetFullPath, FileName);
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
		if (UserId != -1 && Size == Offset + DataCharIndex) {
			wchar_t LogMsg[512] = L"";
			wchar_t LogMsgJa[512] = L"";
			StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_FILEUPLOADED), FileName);
			StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_FILEUPLOADED), FileName);
			StkWebAppUm_AddLogMsg(LogMsg, LogMsgJa, UserId);
		}
	}
	StkObject* TmpObj = new StkObject(L"");
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	*ResultCode = 200;
	return TmpObj;
}
