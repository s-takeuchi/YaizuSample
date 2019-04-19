#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

int main(int argvc, char* argv[])
{
	wchar_t StatusTimeUtc[64];
	wchar_t StatusTimeLocal[64];
	StkObject* NewObj = new StkObject(L"");
	StkPlGetWTimeInIso8601(StatusTimeUtc, false);
	StkPlGetWTimeInIso8601(StatusTimeLocal, true);
	StkObject* AgentInfo = new StkObject(L"AgentInfo");
	AgentInfo->AppendChildElement(new StkObject(L"Name", L"xxx"));
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeUtc", StatusTimeUtc));
	AgentInfo->AppendChildElement(new StkObject(L"StatusTimeLocal", StatusTimeLocal));
	NewObj->AppendChildElement(AgentInfo);
	StkWebAppSend SendObj(1, L"localhost", 8080);
	int Result = 0;
	StkObject* ResObj = SendObj.SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NewObj, &Result);
	return 0;
}
