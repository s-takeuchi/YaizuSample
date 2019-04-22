#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

StkObject* GetAgentInfo()
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
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeUtc", StatusTimeUtc));
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeLocal", StatusTimeLocal));
	NewObj->AppendChildElement(AgentInfo);
	return NewObj;
}

int main(int argc, char* argv[])
{
	wchar_t HostOrIpAddr[256] = L"";
	int PortNum = 0;
	if (argc >= 2) {
		StkPlConvUtf8ToWideChar(HostOrIpAddr, 256, argv[1]);
		PortNum = StkPlAtoi(argv[2]);
	} else {
		StkPlPrintf("Usage: %s, hostname_or_ipaddress, port number\r\n", argv[0]);
		StkPlExit(-1);
	}

	StkWebAppSend SendObj(1, HostOrIpAddr, PortNum);
	int Result = 0;
	SendObj.SetTimeoutInterval(60000 * 16);
	while (true) {
		StkPlPrintf("Get Command For Status...");
		StkObject* ResGetCommandForStatus = SendObj.SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/statuscommand/", NULL, &Result);
		if (Result == 200 && ResGetCommandForStatus != NULL) {
			StkObject* TargetObj = ResGetCommandForStatus->GetFirstChildElement();
			while (TargetObj) {
				if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Timeout") == 0) {
					StkPlPrintf("Timeout\r\n", Result);
				} else if (StkPlWcsCmp(TargetObj->GetName(), L"Msg0") == 0 && StkPlWcsCmp(TargetObj->GetStringValue(), L"Execution") == 0) {
					StkPlPrintf("Execution\r\n", Result);
					StkObject* ResObj = SendObj.SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", GetAgentInfo(), &Result);
					delete ResObj;
				} else {
					StkPlPrintf("Unknown Response\r\n", Result);
				}
				TargetObj = TargetObj->GetNext();
			}
		} else {
			StkPlPrintf("Error[%d]\r\n", Result);
		}
		delete ResGetCommandForStatus;
	}
	
	return 0;
}
