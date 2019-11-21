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

bool TestForGetApi(StkWebAppSend* StkWebAppSendObj, const char* Url, const char* Auth, StkObject* CompObj)
{
	int ResultCode = 0;
	StkWebAppSendObj->SetAutholization(Auth);
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Url, NULL, &ResultCode);
	if (ResultCode != 200) {
		if (ResObj != NULL) {
			delete ResObj;
		}
		return false;
	}
	if (ResObj->Contains(CompObj) == NULL) {
		delete ResObj;
		return false;
	}
	return true;
}

void TestNewAgentInfoNotificationNormal(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (testagent) ... ");
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
	int ErrCode = 0;
	StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"AgentInfo\" : {\"Name\":\"testagent\", \"Status\":-980}}", &ErrCode);
	bool Result = TestForGetApi(StkWebAppSendObj, "/api/agent/", "Bearer admin@a.a manager", ReqObj);
	if (Result != true) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ReqObj;
	StkPlPrintf("[OK]\n");
}

void TestNewAgentInfoNotificationNull(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (NULL) ... ");
	int ResultCode = 0;
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NULL, &ResultCode);
	if (ResultCode != 400 || ResObj == NULL || StkPlWcsStr(ResObj->GetFirstChildElement()->GetStringValue(), L"No ") == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\n");
	delete ResObj;
}

void TestNewAgentInfoNotificationNoAgentInfo(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (no agent info) ... ");
	int ResultCode = 0;
	int ErrCode = 0;
	StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"XXX\" : {\"Name\":\"testagent\", \"Status\":-980}}", &ErrCode);
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", ReqObj, &ResultCode);
	if (ResultCode != 400 || ResObj == NULL || StkPlWcsStr(ResObj->GetFirstChildElement()->GetStringValue(), L"No ") == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\n");
	delete ReqObj;
	delete ResObj;
}

void TestNewAgentInfoNotificationForbiddenChar(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (invalid host name) ... ");
	const wchar_t* HostName[] = { L"abc_xyz", L"あいうえお", L"hello!" };
	wchar_t ReqStr[256] = L"";

	for (int Loop = 0; Loop < 3; Loop++) {
		StkPlSwPrintf(ReqStr, 256, L"{\"AgentInfo\" : {\"Name\":\"%ls\", \"Status\":-980}}", HostName[Loop]);
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqStr, &ErrCode);
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", ReqObj, &ResultCode);
		if (ResultCode != 400 || ResObj == NULL || StkPlWcsStr(ResObj->GetFirstChildElement()->GetStringValue(), L"forbidden ") == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ReqObj;
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

void TestGetCommand(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("Get Status Command ... ");
	int ResultCode1 = 0;
	StkObject* ResObj1 = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/statuscommand/aaa/", NULL, &ResultCode1);
	if (ResultCode1 != 400) {
		if (ResObj1 != NULL) {
			delete ResObj1;
		}
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\n");

	StkPlPrintf("Get Operation Command ... ");
	int ResultCode2 = 0;
	StkObject* ResObj2 = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/opcommand/aaa/", NULL, &ResultCode2);
	if (ResultCode2 != 400) {
		if (ResObj2 != NULL) {
			delete ResObj2;
		}
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}

	if (!ResObj1->Equals(ResObj2)) {
		delete ResObj1;
		delete ResObj2;
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}

	delete ResObj1;
	delete ResObj2;
	StkPlPrintf("[OK]\n");
}

int main(int Argc, char* Argv[])
{
	StkPlSleepMs(3000);
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 10009);
	TestGetUser(StkWebAppSendObj);

	TestNewAgentInfoNotificationNormal(StkWebAppSendObj);
	TestNewAgentInfoNotificationNull(StkWebAppSendObj);
	TestNewAgentInfoNotificationNoAgentInfo(StkWebAppSendObj);
	TestNewAgentInfoNotificationForbiddenChar(StkWebAppSendObj);

	TestGetCommand(StkWebAppSendObj);
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
