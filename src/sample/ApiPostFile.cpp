#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiPostFile.h"

#define CHUNKSIZE_POSTFILE 100000

StkObject* ApiPostFile::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		UserId = -1;
	}
		
	StkObject* TmpObj = new StkObject(L"");

	if (ReqObj != NULL) {
		*ResultCode = 200;
		int Offset = -1;
		int Size = -1;
		wchar_t* FileData = NULL;
		wchar_t* FileName = NULL;
		int Type = -1;
		wchar_t* AgentName = NULL;
		wchar_t* CommandName = NULL;

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
			if (StkPlWcsCmp(CurObj->GetName(), L"Type") == 0) {
				Type = CurObj->GetIntValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"AgentName") == 0) {
				AgentName = CurObj->GetStringValue();
			}
			if (StkPlWcsCmp(CurObj->GetName(), L"CommandName") == 0) {
				CommandName = CurObj->GetStringValue();
			}
			CurObj = CurObj->GetNext();
		}
		if (Offset == -1) {
			CommonError_NoElemInRequest(TmpObj, L"FileOffset");
			*ResultCode = 400;
			return TmpObj;
		}
		if (FileData == NULL) {
			CommonError_NoElemInRequest(TmpObj, L"FileData");
			*ResultCode = 400;
			return TmpObj;
		}
		if (FileName == NULL) {
			CommonError_NoElemInRequest(TmpObj, L"FileName");
			*ResultCode = 400;
			return TmpObj;
		}
		if (Size == -1) {
			CommonError_NoElemInRequest(TmpObj, L"FileSize");
			*ResultCode = 400;
			return TmpObj;
		}
		wchar_t* FileDataPtr = FileData;
		int FileDataLen = (int)StkPlWcsLen(FileData);
		unsigned char* DataChar = new unsigned char[CHUNKSIZE_POSTFILE + 1];
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
			if (DataCharIndex >= CHUNKSIZE_POSTFILE) {
				break;
			}
			DataChar[DataCharIndex] = HexNum[*FileDataPtr] * 16 + HexNum[*(FileDataPtr + 1)];
			DataCharIndex++;
			FileDataPtr += 2;
		}
		DataChar[DataCharIndex] = '\0';

		if (Type == POSTFILE_TYPE_COMMANDRESULT) {
			if (AgentName == NULL) {
				CommonError_NoElemInRequest(TmpObj, L"AgentName");
				*ResultCode = 400;
				return TmpObj;
			}
			if (CommandName == NULL) {
				CommonError_NoElemInRequest(TmpObj, L"CommandName");
				*ResultCode = 400;
				return TmpObj;
			}
			// Write data to database
			DataAccess::GetInstance()->SetCommandResult(AgentName, CommandName, (char*)DataChar, DataCharIndex + 1);
			///////////////////////////////////////////////////////////////////////
			char TBuf[50] = "";
			StkPlSPrintf(TBuf, 50, "file size (1) = %d", DataCharIndex);
			MessageProc::AddLog(TBuf, MessageProc::LOG_TYPE_INFO);
			///////////////////////////////////////////////////////////////////////
		} else {
			// Writing data to file
			void* FilePtr = NULL;
			wchar_t TargetFullPath[FILENAME_MAX];
			GetFullPathFromFileName(TargetFullPath, FileName);
			if (Offset == 0) {
				FilePtr = StkPlOpenFileForWrite(TargetFullPath);
			} else {
				size_t TargetFileSize = StkPlGetFileSize(TargetFullPath);
				if (TargetFileSize != Offset) {
					AddCodeAndMsg(TmpObj, MSG_FILE_INVALID_ORDER, MessageProc::GetMsgEng(MSG_FILE_INVALID_ORDER), MessageProc::GetMsgJpn(MSG_FILE_INVALID_ORDER));
					*ResultCode = 400;
					return TmpObj;
				}
				FilePtr = StkPlOpenFileForWrite(TargetFullPath, true);
			}
			StkPlSeekFromEnd(FilePtr, 0);
			size_t ActSize = 0;
			StkPlWrite(FilePtr, (char*)DataChar, DataCharIndex, &ActSize);
			StkPlCloseFile(FilePtr);

			// Logging
			if (UserId != -1 && Size == Offset + DataCharIndex) {
				wchar_t LogMsg[512] = L"";
				wchar_t LogMsgJa[512] = L"";
				StkPlSwPrintf(LogMsg, 256, L"%ls [%ls]", MessageProc::GetMsgEng(MSG_FILEUPLOADED), FileName);
				StkPlSwPrintf(LogMsgJa, 256, L"%ls [%ls]", MessageProc::GetMsgJpn(MSG_FILEUPLOADED), FileName);
				EventLogging(LogMsg, LogMsgJa, UserId);
			}
		}

		delete [] DataChar;
	}
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	*ResultCode = 200;
	return TmpObj;
}
