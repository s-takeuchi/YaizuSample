#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("GetUser(admin@a.a) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin@a.a manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* ResUserObj = ResObj->GetFirstChildElement();
		if (ResUserObj == NULL || StkPlWcsCmp(ResUserObj->GetName(), L"User") != 0) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}

	{
		StkPlPrintf("GetUser(admin@a.a + Method=OPTIONS) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin@a.a manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_OPTIONS, "/api/user/", NULL, &ResultCode);
		if (ResObj != NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}

	{
		StkPlPrintf("GetUser(dummy) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer dummy dummy");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResultCode != 403) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}
}

void TestNewAgentInfoNotification(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification ... ");
	int ResultCode = 0;

	{
		wchar_t StatusTimeUtc[64];
		wchar_t StatusTimeLocal[64];
		StkObject* NewObj = new StkObject(L"");
		StkPlGetWTimeInIso8601(StatusTimeUtc, false);
		StkPlGetWTimeInIso8601(StatusTimeLocal, true);
		StkObject* AgentInfo = new StkObject(L"AgentInfo");
		AgentInfo->AppendChildElement(new StkObject(L"Name", L"testagent"));
		AgentInfo->AppendChildElement(new StkObject(L"Status", -980));
		AgentInfo->AppendChildElement(new StkObject(L"StatusTimeUtc", StatusTimeUtc));
		AgentInfo->AppendChildElement(new StkObject(L"StatusTimeLocal", StatusTimeLocal));
		NewObj->AppendChildElement(AgentInfo);

		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NewObj, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ResObj;
	}

	{
		StkWebAppSendObj->SetAutholization("Bearer admin@a.a manager"); 
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/agent/", NULL, &ResultCode);
		if (ResultCode != 200 || ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* CurObj = ResObj->GetFirstChildElement()->GetFirstChildElement();
		while (CurObj) {
			if (StkPlWcsCmp(CurObj->GetName(), L"Status") == 0) {
				if (CurObj->GetIntValue() != -980) {
					StkPlPrintf("[NG]\n");
					StkPlExit(1);
				} else {
					break;
				}
			}
			CurObj = CurObj->GetNext();
		}

		delete ResObj;
	}

	StkPlPrintf("[OK]\n");
}

void TestPostOperationStop(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("PostOperationStop ... ");
	int ResultCode = 0;
	StkObject* ReqObj = new StkObject(L"");
	ReqObj->AppendChildElement(new StkObject(L"Operation", L"Stop"));
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
	delete ResObj;
	StkPlPrintf("[OK]\n");
}

int main(int Argc, char* Argv[])
{
	StkPlSleepMs(3000);
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 10009);
	TestGetUser(StkWebAppSendObj);
	TestNewAgentInfoNotification(StkWebAppSendObj);
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
