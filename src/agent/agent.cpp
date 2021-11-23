#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"

#define AGENT_VERSION "1.0.0"

#define TYPE_FILE 0
#define TYPE_COMMANDRESULT 1

#ifdef WIN32
#include <Windows.h>
#define SERVICE_NAME (TEXT("YaizuSampleAgent"))

bool g_bRun = true;
bool g_bService = true;
SERVICE_STATUS_HANDLE g_hServiceStatus = NULL;

DWORD WINAPI HandlerEx(DWORD, DWORD, PVOID, PVOID);
VOID WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv);

SERVICE_TABLE_ENTRY ServiceTable[] = {
	{ SERVICE_NAME, ServiceMain },
{ NULL, NULL }
}; 
#endif

#ifdef WIN32
#define AGT_PLATFORM 1
#else
#define AGT_PLATFORM 0
#endif

#define CHUNKSIZE_FILE 100000

#define RESULTCODE_AGENTSTART            -980
#define RESULTCODE_NOSCRIPT              -981
#define RESULTCODE_OPCOMMANDSTART        -985
#define RESULTCODE_ERROR_SERVERFILE      -990
#define RESULTCODE_ERROR_AGENTFILE       -991
#define RESULTCODE_ERROR_PLATFORM        -992
#define RESULTCODE_ERROR_TIMEOUT         -993
#define RESULTCODE_ERROR_INVALIDAGTDIR   -994
#define RESULTCODE_ERROR_CMDRESULT       -995

StkWebAppSend* SoForTh1 = NULL;
StkWebAppSend* SoForTh2 = NULL;

wchar_t HostName[256];

#define SCRIPT_ENCODE_UTF8 0
#define SCRIPT_ENCODE_SJIS 1
int ScriptEncode = SCRIPT_ENCODE_UTF8;

bool InvalidDirFlag = false;

StkObject* GetAgentInfo(int Status)
{
	StkObject* NewObj = new StkObject(L"");
	long long TmpTime = StkPlGetTime();
	wchar_t TmpTimeStr[17] = L"";
	StkPlSwPrintf(TmpTimeStr, 17, L"%016x", TmpTime);
	NewObj->AppendChildElement(new StkObject(L"Name", HostName));
	NewObj->AppendChildElement(new StkObject(L"Status", Status));
	NewObj->AppendChildElement(new StkObject(L"AcqTime", TmpTimeStr));
	return NewObj;
}

StkObject* GetAgentInfoForOpStatus(int Status)
{
	StkObject* NewObj = new StkObject(L"");
	NewObj->AppendChildElement(new StkObject(L"Name", HostName));
	NewObj->AppendChildElement(new StkObject(L"OpStatus", Status));
	return NewObj;
}

int LoadAndPostFile(char* FileName, int Type, wchar_t* CommandName, int Status, int ExitCode, StkWebAppSend* SndObj)
{
	int ActChunkSize = CHUNKSIZE_FILE;
	if (Type == TYPE_COMMANDRESULT) {
		ActChunkSize = CHUNKSIZE_FILE / 2;
	}

	wchar_t* FileNameWc = StkPlCreateWideCharFromUtf8(FileName);
	int FileSize = (int)StkPlGetFileSize(FileNameWc);
	if (FileSize < 0) {
		return -1;
	}

	int LoopCnt = FileSize / ActChunkSize + 1;
	if (Type == TYPE_COMMANDRESULT && FileSize >= ActChunkSize) {
		FileSize = ActChunkSize;
		LoopCnt = 1;
	}

	int Result = 0;
	for (int LoopChunk = 0; LoopChunk < LoopCnt; LoopChunk++) {
		int Offset = LoopChunk * ActChunkSize;
		char* Buffer = new char[ActChunkSize + 3];
		size_t ActSize;
		void* FilePtr = StkPlOpenFileForRead(FileNameWc);
		if (FilePtr != NULL) {
			StkPlSeekFromBegin(FilePtr, Offset);
			StkPlRead(FilePtr, Buffer, ActChunkSize, &ActSize);
			StkPlCloseFile(FilePtr);
			Buffer[ActSize] = '\0';
			Buffer[ActSize + 1] = '\0';
			Buffer[ActSize + 2] = '\0';
		} else {
			delete Buffer;
			delete FileNameWc;
			return -1;
		}
		if (Type == TYPE_COMMANDRESULT) {
			wchar_t* TmpEncodeScript = NULL;
			if (ScriptEncode == SCRIPT_ENCODE_UTF8) {
				// Nothing to do
			} else if (ScriptEncode == SCRIPT_ENCODE_SJIS) {
				TmpEncodeScript = StkPlSjisToWideChar(Buffer);
				delete Buffer;
				if (TmpEncodeScript) {
					Buffer = StkPlCreateUtf8FromWideChar(TmpEncodeScript);
					ActSize = 0;
					for (char* SizeLoop = Buffer; *SizeLoop != '\0'; SizeLoop++) {
						ActSize++;
					}
					delete TmpEncodeScript;
				} else {
					Buffer = new char[1];
					*Buffer = '\0';
					ActSize = 1;
				}
			}
		}
		
		wchar_t* HexBuf = new wchar_t[ActSize * 2 + 1];
		wchar_t HexChar[16] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
		size_t Loop = 0;
		for (; Loop < ActSize; Loop++) {
			HexBuf[Loop * 2] = HexChar[(unsigned char)Buffer[Loop] / 16];
			HexBuf[Loop * 2 + 1] = HexChar[(unsigned char)Buffer[Loop] % 16];
		}
		HexBuf[Loop * 2] = '\0';
		delete Buffer;

		StkObject* TmpObj = new StkObject(L"");
		TmpObj->AppendChildElement(new StkObject(L"FileName", FileNameWc));
		TmpObj->AppendChildElement(new StkObject(L"FileOffset", Offset));
		TmpObj->AppendChildElement(new StkObject(L"FileSize", FileSize));
		TmpObj->AppendChildElement(new StkObject(L"FileData", HexBuf));
		TmpObj->AppendChildElement(new StkObject(L"Type", Type));
		if (Type == TYPE_COMMANDRESULT) {
			TmpObj->AppendChildElement(new StkObject(L"AgentName", HostName));
			TmpObj->AppendChildElement(new StkObject(L"CommandName", CommandName));
			TmpObj->AppendChildElement(new StkObject(L"Status", Status));
			TmpObj->AppendChildElement(new StkObject(L"ExitCode", ExitCode));
		}
		delete HexBuf;
		StkObject* ResObj = SndObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", TmpObj, &Result);
		delete TmpObj;
		if (ResObj != NULL) {
			delete ResObj;
		}
		if (Result != 200) {
			delete FileNameWc;
			return Result;
		}
	}

	delete FileNameWc;
	return Result;
}

int GetAndSaveFile(char* FileName, size_t FileSize, StkWebAppSend* SndObj)
{
	int Result = 0;

	int LoopCnt = (int)FileSize / CHUNKSIZE_FILE + 1;

	for (int Loop = 0; Loop < LoopCnt; Loop++) {
		char Url[128] = "";
		StkPlSPrintf(Url, 128, "/api/file/%s/%d/", FileName, Loop * CHUNKSIZE_FILE);
		StkObject* ResObj2 = SndObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Url, NULL, &Result);
		if (ResObj2 == NULL) {
			return Result;
		}
		if (Result != 200) {
			delete ResObj2;
			return Result;
		}

		StkObject* CurResObj2 = ResObj2->GetFirstChildElement();
		while (CurResObj2) {
			if (StkPlWcsCmp(CurResObj2->GetName(), L"Data") == 0) {
				CurResObj2 = CurResObj2->GetFirstChildElement();
				continue;
			}
			if (StkPlWcsCmp(CurResObj2->GetName(), L"FileData") == 0) {
				wchar_t* FileData = CurResObj2->GetStringValue();
				wchar_t* FileDataPtr = FileData;
				int FileDataLen = (int)StkPlWcsLen(FileData);
				unsigned char* DataChar = new unsigned char[CHUNKSIZE_FILE + 1];
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
					if (DataCharIndex >= CHUNKSIZE_FILE) {
						break;
					}
					DataChar[DataCharIndex] = HexNum[*FileDataPtr] * 16 + HexNum[*(FileDataPtr + 1)];
					DataCharIndex++;
					FileDataPtr += 2;
				}
				DataChar[DataCharIndex] = '\0';
				void* FilePtr = NULL;
				wchar_t* FileNameTmp = StkPlCreateWideCharFromUtf8(FileName);
				if (Loop == 0) {
					FilePtr = StkPlOpenFileForWrite(FileNameTmp);
				} else {
					FilePtr = StkPlOpenFileForWrite(FileNameTmp, true);
				}
				delete FileNameTmp;
				if (FilePtr != NULL) {
					StkPlSeekFromEnd(FilePtr, 0);
					size_t ActSize = 0;
					StkPlWrite(FilePtr, (char*)DataChar, DataCharIndex, &ActSize);
					StkPlCloseFile(FilePtr);
				} else {
					delete DataChar;
					delete ResObj2;
					return -1;
				}
				delete DataChar;
			}
			CurResObj2 = CurResObj2->GetNext();
		}
		delete ResObj2;
	}
	return Result;
}

int SendCommandResult(bool OperationFlag, wchar_t CmdName[FILENAME_MAX], int Status, int ExitCode, StkWebAppSend* SndObj)
{
	char CmdResultFile[FILENAME_MAX] = "";
	if (OperationFlag) {
		StkPlStrCpy(CmdResultFile, FILENAME_MAX, "aaa-operation.out");
	} else {
		StkPlStrCpy(CmdResultFile, FILENAME_MAX, "aaa-status.out");
	}
	if (LoadAndPostFile(CmdResultFile, TYPE_COMMANDRESULT, CmdName, Status, ExitCode, SndObj) != 200) {
		return RESULTCODE_ERROR_CMDRESULT;
	}
	return Status;
}

int CommonProcess(StkObject* CommandSearch, StkWebAppSend* SndObj, bool OperationFlag)
{
	int ReturnCode = RESULTCODE_NOSCRIPT;
	int ResultFlag = -1;

	while (CommandSearch) {
		if (StkPlWcsCmp(CommandSearch->GetName(), L"Data") == 0) {
			CommandSearch = CommandSearch->GetFirstChildElement();
			continue;
		}

		if (StkPlWcsCmp(CommandSearch->GetName(), L"Command") == 0) {
			ReturnCode = 0;
			ResultFlag = 0;

			char* TmpName = NULL;
			wchar_t CmdName[FILENAME_MAX] = L"";
			char* TmpScript = NULL;
			wchar_t* TmpScriptWc = NULL;
			int TmpType = -1; // 0:Linux, 1:Windows
			char TmpServerFileName[5][FILENAME_MAX] = {"", "", "", "", ""};
			char TmpAgentFileName[5][FILENAME_MAX] = { "", "", "", "", "" };
			int TmpServerFileSize[5] = {-1, -1, -1, -1, -1};
			int TmpServerFileNameCount = 0;
			int TmpAgentFileNameCount = 0;
			int TmpTimeout = 0;
			StkObject* ScriptSearch = CommandSearch->GetFirstChildElement();
			while (ScriptSearch) {
				if (StkPlWcsCmp(ScriptSearch->GetName(), L"Script") == 0) {
					TmpScriptWc = ScriptSearch->GetStringValue();
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Type") == 0) {
					TmpType = ScriptSearch->GetIntValue();
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"ServerFile") == 0) {
					// ServerFile
					if (TmpServerFileNameCount >= 5) {
						ScriptSearch = ScriptSearch->GetNext();
						continue;
					}
					StkObject* ServerFileObj = ScriptSearch->GetFirstChildElement();
					while (ServerFileObj) {
						if (StkPlWcsCmp(ServerFileObj->GetName(), L"ServerFileName") == 0) {
							StkPlConvWideCharToUtf8(TmpServerFileName[TmpServerFileNameCount], FILENAME_MAX, ServerFileObj->GetStringValue());
						} else if (StkPlWcsCmp(ServerFileObj->GetName(), L"ServerFileSize") == 0) {
							TmpServerFileSize[TmpServerFileNameCount] = ServerFileObj->GetIntValue();
						}
						ServerFileObj = ServerFileObj->GetNext();
					}
					TmpServerFileNameCount++;
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Name") == 0) {
					TmpName = StkPlCreateUtf8FromWideChar(ScriptSearch->GetStringValue());
					StkPlWcsCpy(CmdName, FILENAME_MAX, ScriptSearch->GetStringValue());
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"AgentFileName") == 0) {
					if (TmpAgentFileNameCount >= 5) {
						ScriptSearch = ScriptSearch->GetNext();
						continue;
					}
					StkPlConvWideCharToUtf8(TmpAgentFileName[TmpAgentFileNameCount], FILENAME_MAX, ScriptSearch->GetStringValue());
					TmpAgentFileNameCount++;
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Timeout") == 0) {
					TmpTimeout = ScriptSearch->GetIntValue();
				}
				ScriptSearch = ScriptSearch->GetNext();
			}

			if (OperationFlag) {
				StkPlWriteFile(L"aaa-operation.out", (char*)"\0", 1);
			} else {
				StkPlWriteFile(L"aaa-status.out", (char*)"\0", 1);
			}

			// Invalid directory specified
			if (InvalidDirFlag) {
				SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_INVALIDAGTDIR, -1, SndObj);
				return RESULTCODE_ERROR_INVALIDAGTDIR;
			}

			// Encode script according to encode specification.
			if (TmpScriptWc != NULL && StkPlWcsCmp(TmpScriptWc, L"") != 0) {
				if (ScriptEncode == SCRIPT_ENCODE_UTF8) {
					TmpScript = StkPlCreateUtf8FromWideChar(TmpScriptWc);
				} else if (ScriptEncode == SCRIPT_ENCODE_SJIS) {
					TmpScript = StkPlWideCharToSjis(TmpScriptWc);
				} else {
					// Default = UTF8
					TmpScript = StkPlCreateUtf8FromWideChar(TmpScriptWc);
				}
			} else {
				TmpScript = new char[1];
				TmpScript[0] = '\0';
			}

			{
				// Logging begin
				char LogDat[4096] = "";
				StkPlSPrintf(LogDat, 4096, "Cmd received (%s) : [Name=%s]", (OperationFlag)? "OP" : "SA",	TmpName != NULL ? TmpName : "null");
				MessageProc::AddLog(LogDat, MessageProc::LOG_TYPE_INFO);

				bool FndSfFlag = false;
				StkPlStrCpy(LogDat, 4096, "    ");
				for (int Loop = 0; Loop < TmpServerFileNameCount; Loop++) {
					char TmpLog[4096] = "";
					if (TmpServerFileName[Loop] != NULL && *TmpServerFileName[Loop] != '\0') {
						StkPlSPrintf(TmpLog, 4096, "[SFile=%s, Size=%d] ", TmpServerFileName[Loop], TmpServerFileSize[Loop]);
						StkPlStrCat(LogDat, 4096, TmpLog);
						FndSfFlag = true;
					}
				}
				if (FndSfFlag) {
					MessageProc::AddLog(LogDat, MessageProc::LOG_TYPE_INFO);
				}

				// Script
				{
					char TmpLog[4096] = "";
					StkPlStrNCpy(TmpLog, 4096, TmpScript, 50);
					for (int Loop = 0; Loop < StkPlStrLen(TmpLog) + 1; Loop++) {
						if (TmpLog[Loop] == '\r' || TmpLog[Loop] == '\n') {
							TmpLog[Loop] = ' ';
						}
					}
					StkPlSPrintf(LogDat, 4096, "    [Script=%s]", TmpLog);
					MessageProc::AddLog(LogDat, MessageProc::LOG_TYPE_INFO);
				}

				bool FndAfFlag = false;
				StkPlStrCpy(LogDat, 4096, "    ");
				for (int Loop = 0; Loop < TmpAgentFileNameCount; Loop++) {
					char TmpLog[4096] = "";
					if (TmpAgentFileName[Loop] != NULL && *TmpAgentFileName[Loop] != '\0') {
						StkPlSPrintf(TmpLog, 4096, "[AFile=%s] ", TmpAgentFileName[Loop]);
						StkPlStrCat(LogDat, 4096, TmpLog);
						FndAfFlag = true;
					}
				}
				if (FndAfFlag) {
					MessageProc::AddLog(LogDat, MessageProc::LOG_TYPE_INFO);
				}
				// Logging end
			}

			if (TmpName != NULL) {
				delete TmpName;
			}

			// Get and save file
			for (int Loop = 0; Loop < TmpServerFileNameCount; Loop++) {
				if (TmpServerFileName[Loop] != NULL && TmpServerFileSize[Loop] >= 0 && StkPlStrCmp(TmpServerFileName[Loop], "") != 0) {
					if (GetAndSaveFile(TmpServerFileName[Loop], TmpServerFileSize[Loop], SndObj) != 200) {
						delete TmpScript;
						SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_SERVERFILE, -1, SndObj);
						return RESULTCODE_ERROR_SERVERFILE;
					}
				} else if (TmpServerFileName != NULL && StkPlStrCmp(TmpServerFileName[Loop], "") != 0 && TmpServerFileSize[Loop] < 0) {
					delete TmpScript;
					SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_SERVERFILE, -1, SndObj);
					return RESULTCODE_ERROR_SERVERFILE;
				}
			}

			// Platform error detection
			if (TmpType != AGT_PLATFORM) {
				delete TmpScript;
				SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_PLATFORM, -1, SndObj);
				return RESULTCODE_ERROR_PLATFORM;
			}

			// Execute script
			// Generate and execute script
			if (TmpScript != NULL && StkPlStrCmp(TmpScript, "") != 0 && (TmpType == 0 || TmpType == 1)) {
				if (TmpType == 0) {
					if (OperationFlag) {
						StkPlWriteFile(L"aaa-operation.sh", TmpScript, StkPlStrLen(TmpScript));
						StkPlSystem("/usr/bin/chmod 755 aaa-operation.sh");
					} else {
						StkPlWriteFile(L"aaa-status.sh", TmpScript, StkPlStrLen(TmpScript));
						StkPlSystem("/usr/bin/chmod 755 aaa-status.sh");
					}
				} else if (TmpType == 1) {
					if (OperationFlag) {
						StkPlWriteFile(L"aaa-operation.bat", TmpScript, StkPlStrLen(TmpScript));
					} else {
						StkPlWriteFile(L"aaa-status.bat", TmpScript, StkPlStrLen(TmpScript));
					}
				}

				if (TmpType == 0) {
					if (OperationFlag) {
						ReturnCode = StkPlExec(L"/bin/bash -c \"./aaa-operation.sh >& aaa-operation.out\"", TmpTimeout * 1000, &ResultFlag);
					} else {
						ReturnCode = StkPlExec(L"/bin/bash -c \"./aaa-status.sh >& aaa-status.out\"", TmpTimeout * 1000, &ResultFlag);
					}
				} else if (TmpType == 1) {
					if (OperationFlag) {
						ReturnCode = StkPlExec(L"c:\\Windows\\System32\\cmd /c \"aaa-operation.bat > aaa-operation.out\"", TmpTimeout * 1000, &ResultFlag);
					} else {
						ReturnCode = StkPlExec(L"c:\\Windows\\System32\\cmd /c \"aaa-status.bat > aaa-status.out\"", TmpTimeout * 1000, &ResultFlag);
					}
				}
			} else {
				//
			}

			// Timeout error
			if (ReturnCode == -2 && ResultFlag != 0) {
				delete TmpScript;
				SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_TIMEOUT, -1, SndObj);
				return RESULTCODE_ERROR_TIMEOUT;
			}

			// Internal error
			if (ReturnCode == -1 && ResultFlag != 0) {
				delete TmpScript;
				SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_CMDRESULT, -1, SndObj);
				return RESULTCODE_ERROR_CMDRESULT;
			}

			// Load and post file
			for (int Loop = 0; Loop < TmpAgentFileNameCount; Loop++) {
				if (TmpAgentFileName[Loop] != NULL && StkPlStrCmp(TmpAgentFileName[Loop], "") != 0) {
					if (LoadAndPostFile(TmpAgentFileName[Loop], TYPE_FILE, CmdName, -1, -1, SndObj) != 200) {
						delete TmpScript;
						SendCommandResult(OperationFlag, CmdName, RESULTCODE_ERROR_AGENTFILE, ReturnCode, SndObj);
						return RESULTCODE_ERROR_AGENTFILE;
					}
				}
			}

			// Normal end
			if (ReturnCode == 0 && ResultFlag == 0) {
				delete TmpScript;
				SendCommandResult(OperationFlag, CmdName, 0, ReturnCode, SndObj);
				return 0;
			}

			// Failed
			if (ReturnCode != 0 && ResultFlag == 0) {
				delete TmpScript;
				SendCommandResult(OperationFlag, CmdName, 1, ReturnCode, SndObj);
				return 1;
			}

			delete TmpScript;
		}
		CommandSearch = CommandSearch->GetNext();
	}
	return ReturnCode;
}

// Create and generate logs for error
// CmdType [in] : 0=Operation command, 1:Status acquisition command
// Result [in] : HTTP Status code
void Log4Error(int CmdType, int Result)
{
	char TmpLog[128] = "";
	if (Result == 0) {
		StkPlSPrintf(TmpLog, 128, "Cmd error (%s) : Connection error", (CmdType == 0)? "OP" : "SA");
	} else if (Result == 200) {
		StkPlSPrintf(TmpLog, 128, "Cmd error (%s) : Status code = 200 without response data", (CmdType == 0) ? "OP" : "SA");
	} else {
		StkPlSPrintf(TmpLog, 128, "Cmd error (%s) : Status code = %d", (CmdType == 0) ? "OP" : "SA", Result);
	}
	MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_ERROR);
}

// Create and generate logs for response
// CmdType [in] : 0=Operation command, 1:Status acquisition command
// Status [in] : Status
void Log4Result(int CmdType, int Status)
{
	char StatusStr[10] = "";
	switch (Status) {
	case RESULTCODE_NOSCRIPT:
		return;
	case -RESULTCODE_ERROR_SERVERFILE:
		StkPlStrCpy(StatusStr, 10, "SFILE");
		break;
	case -RESULTCODE_ERROR_AGENTFILE:
		StkPlStrCpy(StatusStr, 10, "AFILE");
		break;
	case -RESULTCODE_ERROR_PLATFORM:
		StkPlStrCpy(StatusStr, 10, "PLATF");
		break;
	case -RESULTCODE_ERROR_TIMEOUT:
		StkPlStrCpy(StatusStr, 10, "TIMEO");
		break;
	case -RESULTCODE_ERROR_INVALIDAGTDIR:
		StkPlStrCpy(StatusStr, 10, "AGDIR");
		break;
	case -RESULTCODE_ERROR_CMDRESULT:
		StkPlStrCpy(StatusStr, 10, "CMRLT");
		break;
	case 0:
		StkPlStrCpy(StatusStr, 10, "SUCCS");
		break;
	case 1:
		StkPlStrCpy(StatusStr, 10, "FAILD");
		break;
	default:
		StkPlStrCpy(StatusStr, 10, "UNKNOWN");
		break;
	}
	char TmpLog[64] = "";
	StkPlSPrintf(TmpLog, 128, "Cmd result (%s) : %s", (CmdType == 0) ? "OP" : "SA", StatusStr);
	MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);
}

int OperationLoop(int TargetId)
{
	int Result = 0;

	wchar_t Url[512] = L"";
	char Urlc[512] = "";
	StkPlSwPrintf(Url, 512, L"/api/opcommand/%ls/", HostName);
	StkPlConvWideCharToUtf8(Urlc, 512, Url);
	StkObject* ResGetCommandForOp = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Urlc, NULL, &Result);
	static bool ErrFlag = false;

	if (Result == 200 && ResGetCommandForOp != NULL) {
		ErrFlag = false;
		StkObject* TargetObj = ResGetCommandForOp->GetFirstChildElement();
		while (TargetObj) {
			char TmpTime[64] = "";
			StkPlGetTimeInIso8601(TmpTime, true);
			if (StkPlWcsCmp(TargetObj->GetName(), L"Data") == 0) {
				TargetObj = TargetObj->GetFirstChildElement();
				continue;
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Status") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Timeout") == 0) {
				//
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Status") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Execution") == 0) {
				//

				StkObject* ResObjStart = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfoForOpStatus(RESULTCODE_OPCOMMANDSTART), &Result);
				delete ResObjStart;

				StkObject* CommandSearch = ResGetCommandForOp->GetFirstChildElement();
				int ReturnCode = CommonProcess(CommandSearch, SoForTh2, true);
				Log4Result(0, ReturnCode);

				StkObject* ResObjEnd = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfoForOpStatus(ReturnCode), &Result);
				delete ResObjEnd;
			}
			TargetObj = TargetObj->GetNext();
		}
	} else {
		if (ErrFlag == false) {
			Log4Error(0, Result);
			ErrFlag = true;
		}
		StkPlSleepMs(30000);
	}
	delete ResGetCommandForOp;
	return 0;
}

int StatusLoop(int TargetId)
{
	int Result = 0;

	wchar_t Url[512] = L"";
	char Urlc[512] = "";
	StkPlSwPrintf(Url, 512, L"/api/statuscommand/%ls/", HostName);
	StkPlConvWideCharToUtf8(Urlc, 512, Url);
	StkObject* ResGetCommandForStatus = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Urlc, NULL, &Result);
	static bool ErrFlag = false;

	if (Result == 200 && ResGetCommandForStatus != NULL) {
		ErrFlag = false;
		StkObject* TargetObj = ResGetCommandForStatus->GetFirstChildElement();
		while (TargetObj) {
			char TmpTime[64] = "";
			StkPlGetTimeInIso8601(TmpTime, true);
			if (StkPlWcsCmp(TargetObj->GetName(), L"Data") == 0) {
				TargetObj = TargetObj->GetFirstChildElement();
				continue;
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Status") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Timeout") == 0) {
				//
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Status") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Execution") == 0) {
				//
				StkObject* CommandSearch = ResGetCommandForStatus->GetFirstChildElement();

				int ReturnCode = CommonProcess(CommandSearch, SoForTh1, false);
				Log4Result(1, ReturnCode);

				StkObject* ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(ReturnCode), &Result);
				delete ResObj;
			} else {
				//
			}
			TargetObj = TargetObj->GetNext();
		}
	} else {
		if (ErrFlag == false) {
			Log4Error(1, Result);
			ErrFlag = true;
		}
		StkPlSleepMs(30000);
	}
	delete ResGetCommandForStatus;
	return 0;
}

int LoadPropertyFile(wchar_t HostOrIpAddr[256], int* PortNum, wchar_t PathToBucket[256], wchar_t LcHostName[256], char TrustedCert[256])
{
	char TmpHostOrIpAddr[256] = "";
	char TmpPathToBucket[256] = "";
	char TmpHostName[256] = "";
	char TmpSecureMode[256] = "";
	char TmpScriptEncode[256] = "";
	char TmpLog[256] = "";
	StkProperties *Prop = new StkProperties();
	if (Prop->GetProperties(L"agent.conf") == 0) {
		// For targethost
		if (Prop->GetPropertyStr("targethost", TmpHostOrIpAddr) != 0) {
			MessageProc::AddLog("targethost property is not found.", MessageProc::LOG_TYPE_ERROR);
			return -1;
		}
		StkPlSPrintf(TmpLog, 256, "targethost property = %s", TmpHostOrIpAddr);
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, TmpHostOrIpAddr);

		// For targetport
		if (Prop->GetPropertyInt("targetport", PortNum) != 0) {
			MessageProc::AddLog("targetport property is not found.", MessageProc::LOG_TYPE_ERROR);
			return -1;
		}
		StkPlSPrintf(TmpLog, 256, "targetport property = %d", *PortNum);
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);

		// For pathtobucket
		if (Prop->GetPropertyStr("workdir", TmpPathToBucket) != 0) {
			MessageProc::AddLog("workdir property is not found.", MessageProc::LOG_TYPE_ERROR);
			return -1;
		}
		StkPlSPrintf(TmpLog, 256, "workdir property = %s", TmpPathToBucket);
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);
		StkPlConvUtf8ToWideChar(PathToBucket, 256, TmpPathToBucket);

		// For hostname
		if (Prop->GetPropertyStr("hostname", TmpHostName) != 0) {
			MessageProc::AddLog("hostname property is not found.", MessageProc::LOG_TYPE_ERROR);
			return -1;
		}
		StkPlSPrintf(TmpLog, 256, "hostname property = %s", TmpHostName);
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);
		StkPlConvUtf8ToWideChar(LcHostName, 256, TmpHostName);

		// For trustedcert
		if (Prop->GetPropertyStr("trustedcert", TrustedCert) != 0) {
			//
		}
		StkPlSPrintf(TmpLog, 256, "trustedcert property = %s", TrustedCert);
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);

		// securemode
		bool SecureMode = false;
		if (Prop->GetPropertyStr("securemode", TmpSecureMode) == 0) {
			if (StkPlStrCmp(TmpSecureMode, "true") == 0) {
				SecureMode = true;
			}
			if (StkPlStrCmp(TmpSecureMode, "false") == 0) {
				SecureMode = false;
				StkPlStrCpy(TrustedCert, 256, "");
			}
		}
		StkPlSPrintf(TmpLog, 256, "securemode property = %s", SecureMode ? "true" : "false");
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);

		if (Prop->GetPropertyStr("scriptencode", TmpScriptEncode) == 0) {
			if (StkPlStrCmp(TmpScriptEncode, "UTF8") == 0) {
				ScriptEncode = SCRIPT_ENCODE_UTF8;
			}
			if (StkPlStrCmp(TmpScriptEncode, "SJIS") == 0) {
				ScriptEncode = SCRIPT_ENCODE_SJIS;
			}
		}
		StkPlSPrintf(TmpLog, 256, "scriptencode property = %s", ScriptEncode == SCRIPT_ENCODE_SJIS ? "SJIS" : "UTF8");
		MessageProc::AddLog(TmpLog, MessageProc::LOG_TYPE_INFO);

	} else {
		MessageProc::AddLog("agent.conf is not found.", MessageProc::LOG_TYPE_FATAL);
		return -1;
	}
	return 0;
}

void StartXxx(wchar_t HostOrIpAddr[256], int PortNum, int InvalidDirectory, char TrustedCert[256])
{
	char LogBuf[1024] = "";
	StkPlSPrintf(LogBuf, 1024, "Agent started  [Ver=%s, Build=%s %s]", AGENT_VERSION, __DATE__, __TIME__);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	if (TrustedCert == NULL || StkPlStrCmp(TrustedCert, "") == 0) {
		SoForTh1 = new StkWebAppSend(1, HostOrIpAddr, PortNum);
		SoForTh2 = new StkWebAppSend(2, HostOrIpAddr, PortNum);
	} else {
		SoForTh1 = new StkWebAppSend(1, HostOrIpAddr, PortNum, TrustedCert, NULL);
		SoForTh2 = new StkWebAppSend(2, HostOrIpAddr, PortNum, TrustedCert, NULL);
	}
	SoForTh1->SetTimeoutInterval(60000 * 16);
	SoForTh2->SetTimeoutInterval(60000 * 16);
	SoForTh1->SetSendBufSize(5000000);
	SoForTh1->SetRecvBufSize(5000000);
	SoForTh2->SetSendBufSize(5000000);
	SoForTh2->SetRecvBufSize(5000000);

	bool ErrFlag = false;
	while (true) {
		int Result = 0;
		StkObject* ResObj = NULL;
		if (InvalidDirectory != 0) {
			ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(RESULTCODE_ERROR_INVALIDAGTDIR), &Result);
			InvalidDirFlag = true;
		} else {
			ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(RESULTCODE_AGENTSTART), &Result);
			InvalidDirFlag = false;
		}
		delete ResObj;

		if (Result == 200) {
			StkPlSleepMs(5000);
			break;
		} else {
			if (!ErrFlag) {
				MessageProc::AddLog("Waiting for connection establishment...", MessageProc::LOG_TYPE_ERROR);
				ErrFlag = true;
			}
			StkPlSleepMs(30000);
		}
	}
	if (InvalidDirFlag) {
		MessageProc::AddLog("Wrong work directory (\"AGDIR\")", MessageProc::LOG_TYPE_ERROR);
	} else {
		MessageProc::AddLog("Initial notification done (\"START\")", MessageProc::LOG_TYPE_INFO);
	}
	AddStkThread(1, L"StatusLoop", L"", NULL, NULL, StatusLoop, NULL, NULL);
	AddStkThread(2, L"OperationLoop", L"", NULL, NULL, OperationLoop, NULL, NULL);
	StartAllOfStkThreads();
}

void LoadPropertyFileAndStart()
{
	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	wchar_t PathToBucket[256] = L"";
	char TrustedCert[256] = "";

	wchar_t LoggingPath[FILENAME_MAX] = L"";
#ifdef WIN32
	StkPlGetFullPathFromFileName(L"servalagt.log", LoggingPath);
#else
	StkPlWcsCpy(LoggingPath, FILENAME_MAX, L"/var/log/servalagt.log");
#endif
	MessageProc::StartLogging(LoggingPath);
	MessageProc::AddLog("----------------------------------------", MessageProc::LOG_TYPE_INFO);

	LoadPropertyFile(HostOrIpAddr, &PortNum, PathToBucket, HostName, TrustedCert);
	if (HostName == NULL || StkPlWcsCmp(HostName, L"") == 0) {
		StkPlGetHostName(HostName, 256);
	}
	int InvalidDirectory = 0;
	if (StkPlWcsCmp(PathToBucket, L"") != 0) {
		InvalidDirectory = ChangeCurrentDirectory(PathToBucket);
	}
	StartXxx(HostOrIpAddr, PortNum, InvalidDirectory, TrustedCert);
}

int main(int argc, char* argv[])
{
	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	wchar_t PathToBucket[256] = L"";
	if (argc >= 5) {
		//
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, argv[1]);
		PortNum = StkPlAtoi(argv[2]);
		StkPlConvUtf8ToWideChar(PathToBucket, 256, argv[3]);
		int InvalidDirectory = 0;
		if (StkPlWcsCmp(PathToBucket, L"") != 0) {
			InvalidDirectory = ChangeCurrentDirectory(PathToBucket);
		}
		StkPlConvUtf8ToWideChar(HostName, 256, argv[4]);
#ifdef WIN32
		ScriptEncode = SCRIPT_ENCODE_SJIS;
#else
		ScriptEncode = SCRIPT_ENCODE_UTF8;
#endif
		if (argc == 6) {
			StartXxx(HostOrIpAddr, PortNum, InvalidDirectory, argv[5]);
		} else {
			StartXxx(HostOrIpAddr, PortNum, InvalidDirectory, NULL);
		}
		while (true) { StkPlSleepMs(1000); }
	} else if (argc == 2 && StkPlStrCmp(argv[1], "help") == 0) {
		StkPlPrintf("Usage: %s  destination_host_name_or_IP_addres  port_number  path_to_bucket  host_name  [trusted_certificate]\r\n", argv[0]);
	} else if (argc == 1) {
#ifdef WIN32
		StartServiceCtrlDispatcher(ServiceTable);
#else
		LoadPropertyFileAndStart();
		while (true) { StkPlSleepMs(1000); }
#endif
	} else {
		StkPlPrintf("Command option error.  Execute '%s help' to refer to command options.\r\n", argv[0]);
	}

	return 0;
}

#ifdef WIN32
DWORD WINAPI HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	// Initialize Variables for Service Control
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 1;
	ss.dwWaitHint = 1000;
	ss.dwControlsAccepted = 0;

	switch (dwControl) {
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:

		// Set STOP_PENDING status.
		ss.dwCurrentState = SERVICE_STOP_PENDING;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}

		// SERVICE SPECIFIC STOPPING CODE HERE.
		// ...
		// ...
		g_bService = false;
		Sleep(2 * 1000);

		// Set STOPPED status.
		ss.dwCurrentState = SERVICE_STOPPED;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	case SERVICE_CONTROL_PAUSE:

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_PAUSE_PENDING;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}

		// APPLICATION SPECIFIC PAUSE_PENDING CODE HERE.
		// ...
		// ...
		g_bRun = false;
		Sleep(2 * 1000);

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_PAUSED;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		ss.dwControlsAccepted =
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN |
			SERVICE_ACCEPT_STOP;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	case SERVICE_CONTROL_CONTINUE:

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_START_PENDING;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}

		// APPLICATION SPECIFIC START_PENDING CODE HERE.
		// ...
		// ...
		g_bRun = true;
		Sleep(2 * 1000);

		// Set RUNNING status.
		ss.dwCurrentState = SERVICE_RUNNING;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		ss.dwControlsAccepted =
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN |
			SERVICE_ACCEPT_STOP;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	default:
		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	return NO_ERROR;
}

VOID WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv)
{
	// Initialize Variables for Service Control
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 1;
	ss.dwWaitHint = 1000;
	ss.dwControlsAccepted = 0;
	ss.dwCurrentState = SERVICE_START_PENDING;

	// Register Service Control Handler
	g_hServiceStatus = RegisterServiceCtrlHandlerEx(SERVICE_NAME, HandlerEx, NULL);
	if (0 == g_hServiceStatus) {
		return;
	}

	// Entering Starting Service.
	if (!SetServiceStatus(g_hServiceStatus, &ss)) {
		return;
	}

	// APPLICATION SPECIFIC INITIALIZATION CODE
	// ...
	// ...

	// Finish Initializing.
	ss.dwCurrentState = SERVICE_RUNNING;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	ss.dwControlsAccepted =
		SERVICE_ACCEPT_PAUSE_CONTINUE |
		SERVICE_ACCEPT_SHUTDOWN |
		SERVICE_ACCEPT_STOP;
	if (!SetServiceStatus(g_hServiceStatus, &ss)) {
		return;
	}

	LoadPropertyFileAndStart();

	while (g_bService) {
		if (!g_bRun) {
			Sleep(1000);
			continue;
		}
		Sleep(1);
	}
}
#endif
