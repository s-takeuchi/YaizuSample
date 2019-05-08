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

StkWebAppSend* SoForTh1 = NULL;
StkWebAppSend* SoForTh2 = NULL;

StkObject* GetAgentInfo(int Status)
{
	wchar_t StatusTimeUtc[64];
	wchar_t StatusTimeLocal[64];
	wchar_t HostName[256];
	StkPlGetHostName(HostName, 256);
	StkObject* NewObj = new StkObject(L"");
	StkPlGetWTimeInIso8601(StatusTimeUtc, false);
	StkPlGetWTimeInIso8601(StatusTimeLocal, true);
	StkObject* AgentInfo = new StkObject(L"AgentInfo");
	AgentInfo->AppendChildElement(new StkObject(L"Name", HostName));
	AgentInfo->AppendChildElement(new StkObject(L"Status", Status));
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeUtc", StatusTimeUtc));
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeLocal", StatusTimeLocal));
	NewObj->AppendChildElement(AgentInfo);
	return NewObj;
}

StkObject* GetAgentInfoForOpCmd()
{
	wchar_t StatusTimeUtc[64];
	wchar_t StatusTimeLocal[64];
	wchar_t HostName[256];
	StkPlGetHostName(HostName, 256);
	StkObject* NewObj = new StkObject(L"");
	StkPlGetWTimeInIso8601(StatusTimeUtc, false);
	StkPlGetWTimeInIso8601(StatusTimeLocal, true);
	StkObject* AgentInfo = new StkObject(L"AgentInfo");
	AgentInfo->AppendChildElement(new StkObject(L"Name", HostName));
	AgentInfo->AppendChildElement(new StkObject(L"OpCmd", -1));
	NewObj->AppendChildElement(AgentInfo);
	return NewObj;
}

int GetAndSaveFile(char* FileName, size_t FileSize)
{
	int Result = 0;

	int LoopCnt = FileSize / 1000000 + 1;

	for (int Loop = 0; Loop < LoopCnt; Loop++) {
		char Url[128] = "";
		StkPlSPrintf(Url, 128, "/api/file/%s/%d/", FileName, Loop * 1000000);
		StkObject* ResObj2 = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Url, NULL, &Result);

		StkObject* CurResObj2 = ResObj2->GetFirstChildElement();
		while (CurResObj2) {
			if (StkPlWcsCmp(CurResObj2->GetName(), L"FileData") == 0) {
				wchar_t* FileData = CurResObj2->GetStringValue();
				wchar_t* FileDataPtr = FileData;
				int FileDataLen = StkPlWcsLen(FileData);
				unsigned char DataChar[1000001];
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
				if (Loop == 0) {
					FilePtr = StkPlOpenFileForWrite(L"bbb.txt");
				} else {
					FilePtr = StkPlOpenFileForWrite(L"bbb.txt", true);
				}
				StkPlSeekFromEnd(FilePtr, 0);
				size_t ActSize = 0;
				StkPlWrite(FilePtr, (char*)DataChar, DataCharIndex, &ActSize);
				StkPlCloseFile(FilePtr);
			}
			CurResObj2 = CurResObj2->GetNext();
		}
		delete ResObj2;
	}
	return Result;
}

int OperationLoop(int TargetId)
{
	int Result = 0;

	wchar_t HostName[256] = L"";
	wchar_t Url[512] = L"";
	char Urlc[512] = "";
	StkPlGetHostName(HostName, 256);
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
				StkPlPrintf("Get Command For Operation >> Execution [%s]\r\n", TmpTime);
				int ReturnCode = 0;
				StkObject* CommandSearch = ResGetCommandForOp->GetFirstChildElement();
				while (CommandSearch) {
					if (StkPlWcsCmp(CommandSearch->GetName(), L"Command") == 0) {
						char* TmpScript = NULL;
						int TmpType = -1;
						char TmpServerFileName[FILENAME_MAX] = "";
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
							}
							ScriptSearch = ScriptSearch->GetNext();
						}
						// Get and save file
						if (TmpServerFileName != NULL && TmpServerFileSize >= 0 && StkPlStrCmp(TmpServerFileName, "") != 0) {
							GetAndSaveFile(TmpServerFileName, TmpServerFileSize);
						}
						if (TmpScript != NULL) {
							if (TmpType == 0) {
								StkPlWriteFile(L"aaa-operation.sh", TmpScript, StkPlStrLen(TmpScript));
							} else if (TmpType == 1) {
								StkPlWriteFile(L"aaa-operation.bat", TmpScript, StkPlStrLen(TmpScript));
							}
							delete TmpScript;

							if (TmpType == 0) {
								ReturnCode = StkPlSystem("/usr/bin/bash aaa-operation.sh");
							} else if (TmpType == 1) {
								ReturnCode = StkPlSystem("cmd /c aaa-operation.bat");
							}
						}
					}
					CommandSearch = CommandSearch->GetNext();
				}
				StkObject* ResObj = SoForTh2->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfoForOpCmd(), &Result);
				delete ResObj;
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

	wchar_t HostName[256] = L"";
	wchar_t Url[512] = L"";
	char Urlc[512] = "";
	StkPlGetHostName(HostName, 256);
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
				StkPlPrintf("Get Command For Status >> Execution [%s]\r\n", TmpTime);
				int ReturnCode = 0;
				StkObject* CommandSearch = ResGetCommandForStatus->GetFirstChildElement();
				while (CommandSearch) {
					if (StkPlWcsCmp(CommandSearch->GetName(), L"Command") == 0) {
						char* TmpScript = NULL;
						int TmpType = -1;
						StkObject* ScriptSearch = CommandSearch->GetFirstChildElement();
						while (ScriptSearch) {
							if (StkPlWcsCmp(ScriptSearch->GetName(), L"Script") == 0) {
								TmpScript = StkPlCreateUtf8FromWideChar(ScriptSearch->GetStringValue());
							} else if (StkPlWcsCmp(ScriptSearch->GetName(), L"Type") == 0) {
								TmpType = ScriptSearch->GetIntValue();
							}
							ScriptSearch = ScriptSearch->GetNext();
						}
						if (TmpScript != NULL) {
							if (TmpType == 0) {
								StkPlWriteFile(L"aaa-status.sh", TmpScript, StkPlStrLen(TmpScript));
							} else if (TmpType == 1) {
								StkPlWriteFile(L"aaa-status.bat", TmpScript, StkPlStrLen(TmpScript));
							}
							delete TmpScript;

							if (TmpType == 0) {
								ReturnCode = StkPlSystem("/usr/bin/bash aaa-status.sh");
							} else if (TmpType == 1) {
								ReturnCode = StkPlSystem("cmd /c aaa-status.bat");
							}
						}
					}
					CommandSearch = CommandSearch->GetNext();
				}
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

int LoadPropertyFile(wchar_t HostOrIpAddr[256], int* PortNum)
{
	char TmpHostOrIpAddr[256] = "";
	StkProperties *Prop = new StkProperties();
	if (Prop->GetProperties(L"agent.conf") == 0) {
		if (Prop->GetPropertyStr("targethost", TmpHostOrIpAddr) != 0) {
			StkPlPrintf("targethost property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("targethost property = %s\r\n", TmpHostOrIpAddr);
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, TmpHostOrIpAddr);

		if (Prop->GetPropertyInt("targetport", PortNum) != 0) {
			StkPlPrintf("targetport property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("targetport property = %d\r\n", *PortNum);
	} else {
		StkPlPrintf("agent.conf is not found.\r\n");
		return -1;
	}
	return 0;
}

void StartXxx(wchar_t HostOrIpAddr[256], int PortNum)
{
	SoForTh1 = new StkWebAppSend(1, HostOrIpAddr, PortNum);
	SoForTh2 = new StkWebAppSend(2, HostOrIpAddr, PortNum);
	SoForTh1->SetTimeoutInterval(60000 * 16);
	SoForTh2->SetTimeoutInterval(60000 * 16);
	SoForTh1->SetSendBufSize(5000000);
	SoForTh1->SetRecvBufSize(5000000);
	SoForTh2->SetSendBufSize(5000000);
	SoForTh2->SetRecvBufSize(5000000);

	int Result = 0;
	StkObject* ResObj = SoForTh1->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(0), &Result);
	delete ResObj;

	AddStkThread(1, L"StatusLoop", L"", NULL, NULL, StatusLoop, NULL, NULL);
	AddStkThread(2, L"OperationLoop", L"", NULL, NULL, OperationLoop, NULL, NULL);
	StartAllOfStkThreads();
}

int main(int argc, char* argv[])
{
	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	if (argc == 3) {
		StkPlPrintf("%s command execution...\r\n", argv[0]);
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, argv[1]);
		PortNum = StkPlAtoi(argv[2]);
		StartXxx(HostOrIpAddr, PortNum);
		while (true) { StkPlSleepMs(1000); }
	} else {
#ifdef WIN32
		StartServiceCtrlDispatcher(ServiceTable);
#else
		LoadPropertyFile(HostOrIpAddr, &PortNum); 
		StartXxx(HostOrIpAddr, PortNum);
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

	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	LoadPropertyFile(HostOrIpAddr, &PortNum);
	StartXxx(HostOrIpAddr, PortNum);

	while (g_bService) {
		if (!g_bRun) {
			Sleep(1000);
			continue;
		}
		Sleep(1);
	}
}
#endif
