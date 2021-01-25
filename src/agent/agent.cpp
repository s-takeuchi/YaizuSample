#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"

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

#define RESULTCODE_AGENTSTART            -980
#define RESULTCODE_NOSCRIPT              -981
#define RESULTCODE_OPCOMMANDSTART        -985
#define RESULTCODE_ERROR_SERVERFILE      -990
#define RESULTCODE_ERROR_AGENTFILE       -991
#define RESULTCODE_ERROR_PLATFORM        -992
#define RESULTCODE_ERROR_INVALIDAGTDIR   -994

StkWebAppSend* SoForTh1 = NULL;
StkWebAppSend* SoForTh2 = NULL;

wchar_t HostName[256];

StkObject* GetAgentInfo(int Status)
{
	wchar_t StatusTimeUtc[64];
	wchar_t StatusTimeLocal[64];
	StkObject* NewObj = new StkObject(L"");
	StkPlGetWTimeInIso8601(StatusTimeUtc, false);
	StkPlGetWTimeInIso8601(StatusTimeLocal, true);
	NewObj->AppendChildElement(new StkObject(L"Name", HostName));
	NewObj->AppendChildElement(new StkObject(L"Status", Status));
	NewObj->AppendChildElement(new StkObject(L"StatusTimeUtc", StatusTimeUtc));
	NewObj->AppendChildElement(new StkObject(L"StatusTimeLocal", StatusTimeLocal));
	return NewObj;
}

StkObject* GetAgentInfoForOpStatus(int Status)
{
	StkObject* NewObj = new StkObject(L"");
	NewObj->AppendChildElement(new StkObject(L"Name", HostName));
	NewObj->AppendChildElement(new StkObject(L"OpStatus", Status));
	return NewObj;
}

int LoadAndPostFile(char* FileName, StkWebAppSend* SndObj)
{
	wchar_t* FileNameWc = StkPlCreateWideCharFromUtf8(FileName);
	int FileSize = (int)StkPlGetFileSize(FileNameWc);
	if (FileSize < 0) {
		return -1;
	}

	int LoopCnt = FileSize / 1000000 + 1;

	int Result = 0;
	for (int LoopChunk = 0; LoopChunk < LoopCnt; LoopChunk++) {
		int Offset = LoopChunk * 1000000;
		char* Buffer = new char[1000000];
		wchar_t* HexBuf = new wchar_t[2000001];
		size_t ActSize;
		void* FilePtr = StkPlOpenFileForRead(FileNameWc);
		if (FilePtr != NULL) {
			StkPlSeekFromBegin(FilePtr, Offset);
			StkPlRead(FilePtr, Buffer, 1000000, &ActSize);
			StkPlCloseFile(FilePtr);
		} else {
			delete Buffer;
			delete FileNameWc;
			return -1;
		}
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
		TmpObj->AppendChildElement(new StkObject(L"FileData", HexBuf));
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

	int LoopCnt = (int)FileSize / 1000000 + 1;

	for (int Loop = 0; Loop < LoopCnt; Loop++) {
		char Url[128] = "";
		StkPlSPrintf(Url, 128, "/api/file/%s/%d/", FileName, Loop * 1000000);
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

int CommonProcess(StkObject* CommandSearch, char TmpTime[64], StkWebAppSend* SndObj, bool OperationFlag)
{
	int ReturnCode = RESULTCODE_NOSCRIPT;
	while (CommandSearch) {
		if (StkPlWcsCmp(CommandSearch->GetName(), L"Data") == 0) {
			CommandSearch = CommandSearch->GetFirstChildElement();
			continue;
		}

		if (StkPlWcsCmp(CommandSearch->GetName(), L"Command") == 0) {
			char* TmpName = NULL;
			char* TmpScript = NULL;
			int TmpType = -1;
			char TmpServerFileName[FILENAME_MAX] = "";
			char TmpAgentFileName[FILENAME_MAX] = "";
			int TmpServerFileSize = -1;
			StkObject* ScriptSearch = CommandSearch->GetFirstChildElement();
			while (ScriptSearch) {
				if (StkPlWcsCmp(ScriptSearch->GetName(), L"Script") == 0) {
					TmpScript = StkPlCreateUtf8FromWideChar(ScriptSearch->GetStringValue());
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Type") == 0) {
					TmpType = ScriptSearch->GetIntValue();
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"ServerFileName") == 0) {
					StkPlConvWideCharToUtf8(TmpServerFileName, FILENAME_MAX, ScriptSearch->GetStringValue());
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"ServerFileSize") == 0) {
					TmpServerFileSize = ScriptSearch->GetIntValue();
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Name") == 0) {
					TmpName = StkPlCreateUtf8FromWideChar(ScriptSearch->GetStringValue());
				} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"AgentFileName") == 0) {
					StkPlConvWideCharToUtf8(TmpAgentFileName, FILENAME_MAX, ScriptSearch->GetStringValue());
				}
				ScriptSearch = ScriptSearch->GetNext();
			}
			// Name
			StkPlPrintf("Name=%s, ", TmpName != NULL ? TmpName : "null");
			if (TmpName != NULL) {
				delete TmpName;
			}

			// Get and save file
			StkPlPrintf("ServerFileName=%s, ", TmpServerFileName != NULL ? TmpServerFileName : "null");
			StkPlPrintf("ServerFileSize=%d\r\n", TmpServerFileSize);
			if (TmpServerFileName != NULL && TmpServerFileSize >= 0 && StkPlStrCmp(TmpServerFileName, "") != 0) {
				if (GetAndSaveFile(TmpServerFileName, TmpServerFileSize, SndObj) != 200) {
					return RESULTCODE_ERROR_SERVERFILE;
				}
			} else if (TmpServerFileName != NULL && StkPlStrCmp(TmpServerFileName, "") != 0 && TmpServerFileSize < 0) {
				return RESULTCODE_ERROR_SERVERFILE;
			}

			// Execute script
			if (TmpScript != NULL && StkPlStrCmp(TmpScript, "") != 0 && (TmpType == 0 || TmpType == 1)) {
				StkPlPrintf("Execute script:\r\n");
				if (TmpType != AGT_PLATFORM) {
					return RESULTCODE_ERROR_PLATFORM;
				}
				if (TmpType == 0) {
					if (OperationFlag) {
						StkPlWriteFile(L"aaa-operation.sh", TmpScript, StkPlStrLen(TmpScript));
					} else {
						StkPlWriteFile(L"aaa-status.sh", TmpScript, StkPlStrLen(TmpScript));
					}
				} else if (TmpType == 1) {
					if (OperationFlag) {
						StkPlWriteFile(L"aaa-operation.bat", TmpScript, StkPlStrLen(TmpScript));
					} else {
						StkPlWriteFile(L"aaa-status.bat", TmpScript, StkPlStrLen(TmpScript));
					}
				}
				delete TmpScript;

				if (TmpType == 0) {
					if (OperationFlag) {
						ReturnCode = StkPlSystem("/usr/bin/bash aaa-operation.sh");
					} else {
						ReturnCode = StkPlSystem("/usr/bin/bash aaa-status.sh");
					}
				} else if (TmpType == 1) {
					if (OperationFlag) {
						ReturnCode = StkPlSystem("cmd /c aaa-operation.bat");
					} else {
						ReturnCode = StkPlSystem("cmd /c aaa-status.bat");
					}
				}
			} else {
				StkPlPrintf("No script is presented.\r\n");
			}

			// Load and post file
			StkPlPrintf("AgentFileName=%s\r\n", TmpAgentFileName != NULL ? TmpAgentFileName : "null");
			if (TmpAgentFileName != NULL && StkPlStrCmp(TmpAgentFileName, "") != 0) {
				if (LoadAndPostFile(TmpAgentFileName, SndObj) != 200) {
					return RESULTCODE_ERROR_AGENTFILE;
				}
			}
		}
		CommandSearch = CommandSearch->GetNext();
	}
	return ReturnCode;
}

int OperationLoop(int TargetId)
{
	int Result = 0;

	wchar_t Url[512] = L"";
	char Urlc[512] = "";
	StkPlSwPrintf(Url, 512, L"/api/opcommand/%ls/", HostName);
	StkPlConvWideCharToUtf8(Urlc, 512, Url);
	StkObject* ResGetCommandForOp = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Urlc, NULL, &Result);

	if (Result == 200 && ResGetCommandForOp != NULL) {
		StkObject* TargetObj = ResGetCommandForOp->GetFirstChildElement();
		while (TargetObj) {
			char TmpTime[64] = "";
			StkPlGetTimeInIso8601(TmpTime, true);
			if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Timeout") == 0) {
				StkPlPrintf("Get Command For Operation >> Timeout [%s]\r\n", TmpTime);
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Execution") == 0) {
				StkPlPrintf("Get Command For Operation >> Execute [%s]\r\n", TmpTime);

				StkObject* ResObjStart = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfoForOpStatus(RESULTCODE_OPCOMMANDSTART), &Result);
				delete ResObjStart;

				StkObject* CommandSearch = ResGetCommandForOp->GetFirstChildElement();
				int ReturnCode = CommonProcess(CommandSearch, TmpTime, SoForTh2, true);

				StkObject* ResObjEnd = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfoForOpStatus(ReturnCode), &Result);
				delete ResObjEnd;
			}
			TargetObj = TargetObj->GetNext();
		}
	} else {
		char TmpTime[64] = "";
		StkPlGetTimeInIso8601(TmpTime, true);
		StkPlPrintf("Get Command For Operation >> Error(%d) [%s]\r\n", Result, TmpTime);
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

	if (Result == 200 && ResGetCommandForStatus != NULL) {
		StkObject* TargetObj = ResGetCommandForStatus->GetFirstChildElement();
		while (TargetObj) {
			char TmpTime[64] = "";
			StkPlGetTimeInIso8601(TmpTime, true);
			if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Timeout") == 0) {
				StkPlPrintf("Get Command For Status >> Timeout [%s]\r\n", TmpTime);
			} else if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Execution") == 0) {
				StkPlPrintf("Get Command For Status >> Execute [%s]\r\n", TmpTime);
				StkObject* CommandSearch = ResGetCommandForStatus->GetFirstChildElement();

				int ReturnCode = CommonProcess(CommandSearch, TmpTime, SoForTh1, false);

				StkObject* ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(ReturnCode), &Result);
				delete ResObj;
			} else {
				//
			}
			TargetObj = TargetObj->GetNext();
		}
	} else {
		char TmpTime[64] = "";
		StkPlGetTimeInIso8601(TmpTime, true);
		StkPlPrintf("Get Command For Status >> Error(%d) [%s]\r\n", Result, TmpTime);
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
	StkProperties *Prop = new StkProperties();
	if (Prop->GetProperties(L"agent.conf") == 0) {
		// For targethost
		if (Prop->GetPropertyStr("targethost", TmpHostOrIpAddr) != 0) {
			StkPlPrintf("targethost property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("targethost property = %s\r\n", TmpHostOrIpAddr);
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, TmpHostOrIpAddr);

		// For targetport
		if (Prop->GetPropertyInt("targetport", PortNum) != 0) {
			StkPlPrintf("targetport property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("targetport property = %d\r\n", *PortNum);

		// For pathtobucket
		if (Prop->GetPropertyStr("workdir", TmpPathToBucket) != 0) {
			StkPlPrintf("workdir property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("workdir property = %s\r\n", TmpPathToBucket);
		StkPlConvUtf8ToWideChar(PathToBucket, 256, TmpPathToBucket);

		// For hostname
		if (Prop->GetPropertyStr("hostname", TmpHostName) != 0) {
			StkPlPrintf("hostname property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("hostname property = %s\r\n", TmpHostName);
		StkPlConvUtf8ToWideChar(LcHostName, 256, TmpHostName);

		// For trustedcert
		if (Prop->GetPropertyStr("trustedcert", TrustedCert) != 0) {
			//
		}
		StkPlPrintf("trustedcert property = %s\r\n", TrustedCert);

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
		StkPlPrintf("securemode property = %s\r\n", SecureMode ? "true" : "false");

	} else {
		StkPlPrintf("agent.conf is not found.\r\n");
		return -1;
	}
	return 0;
}

void StartXxx(wchar_t HostOrIpAddr[256], int PortNum, int InvalidDirectory, char TrustedCert[256])
{
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

	if (InvalidDirectory != 0) {
		int Result = 0;
		StkObject* ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(RESULTCODE_ERROR_INVALIDAGTDIR), &Result);
		delete ResObj;
	} else {
		int Result = 0;
		StkObject* ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(RESULTCODE_AGENTSTART), &Result);
		delete ResObj;

		AddStkThread(1, L"StatusLoop", L"", NULL, NULL, StatusLoop, NULL, NULL);
		AddStkThread(2, L"OperationLoop", L"", NULL, NULL, OperationLoop, NULL, NULL);
		StartAllOfStkThreads();
	}
}

void LoadPropertyFileAndStart()
{
	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	wchar_t PathToBucket[256] = L"";
	char TrustedCert[256] = "";

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
		StkPlPrintf("Agent starts\r\n");
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, argv[1]);
		PortNum = StkPlAtoi(argv[2]);
		StkPlConvUtf8ToWideChar(PathToBucket, 256, argv[3]);
		int InvalidDirectory = 0;
		if (StkPlWcsCmp(PathToBucket, L"") != 0) {
			InvalidDirectory = ChangeCurrentDirectory(PathToBucket);
		}
		StkPlConvUtf8ToWideChar(HostName, 256, argv[4]);
		if (argc == 6) {
			StartXxx(HostOrIpAddr, PortNum, InvalidDirectory, argv[5]);
		} else {
			StartXxx(HostOrIpAddr, PortNum, InvalidDirectory, NULL);
		}
		while (true) { StkPlSleepMs(1000); }
	} else {
		StkPlPrintf("Usage: %s  destination_host_name_or_IP_addres  port_number  path_to_bucket  host_name  [trusted_certificate]", argv[0]);
#ifdef WIN32
		StartServiceCtrlDispatcher(ServiceTable);
#else
		LoadPropertyFileAndStart();
		while (true) { StkPlSleepMs(1000); }
#endif
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
