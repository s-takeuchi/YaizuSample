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

StkWebAppSend* SendObjPostAgentInfo = NULL;
StkWebAppSend* SendObjGetStatusCommand = NULL;

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

int OperationLoop(int TargetId)
{
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
	StkObject* ResGetCommandForStatus = SendObjGetStatusCommand->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Urlc, NULL, &Result);

	if (Result == 200 && ResGetCommandForStatus != NULL) {
		StkObject* TargetObj = ResGetCommandForStatus->GetFirstChildElement();
		while (TargetObj) {
			char TmpTime[64] = "";
			StkPlGetTimeInIso8601(TmpTime, false);
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
								StkPlWriteFile(L"aaa.sh", TmpScript, StkPlStrLen(TmpScript));
							} else if (TmpType == 1) {
								StkPlWriteFile(L"aaa.bat", TmpScript, StkPlStrLen(TmpScript));
							}
							delete TmpScript;

							if (TmpType == 0) {
								ReturnCode = StkPlSystem("/usr/bin/bash aaa.sh");
							} else if (TmpType == 1) {
								ReturnCode = StkPlSystem("cmd /c aaa.bat");
							}
						}
					}
					CommandSearch = CommandSearch->GetNext();
				}
				StkObject* ResObj = SendObjPostAgentInfo->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(ReturnCode), &Result);
				delete ResObj;
			} else {
				//
			}
			TargetObj = TargetObj->GetNext();
		}
	} else {
		StkPlPrintf("Error[%d]\r\n", Result);
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
	SendObjPostAgentInfo = new StkWebAppSend(2, HostOrIpAddr, PortNum);
	SendObjGetStatusCommand = new StkWebAppSend(3, HostOrIpAddr, PortNum);
	SendObjPostAgentInfo->SetTimeoutInterval(60000 * 16);
	SendObjGetStatusCommand->SetTimeoutInterval(60000 * 16);

	int Result = 0;
	StkObject* ResObj = SendObjPostAgentInfo->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(0), &Result);
	delete ResObj;

	AddStkThread(1, L"Loop", L"", NULL, NULL, StatusLoop, NULL, NULL);
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
