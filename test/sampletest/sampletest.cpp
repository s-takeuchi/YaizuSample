﻿#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("GetUser(admin) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* ResUserObj = ResObj->GetFirstChildElement()->GetFirstChildElement();
		if (ResUserObj == NULL || StkPlWcsCmp(ResUserObj->GetName(), L"User") != 0) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}

	{
		StkPlPrintf("GetUser(admin + Method=OPTIONS) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
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
		if (ResultCode != 401) {
			StkPlPrintf("[NG] Res=%d\n", ResultCode);
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
		StkObject* NewObj = new StkObject(L"");
		NewObj->AppendChildElement(new StkObject(L"Name", L"testagent"));
		NewObj->AppendChildElement(new StkObject(L"Status", -980));
		NewObj->AppendChildElement(new StkObject(L"AcqTime", L"00000000FFFFFFFF"));

		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NewObj, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ResObj;
		delete NewObj;
	}
	int ErrCode = 0;
	StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{ \"Data\" : {\"AgentInfo\" : {\"Name\":\"testagent\", \"Status\":-980, \"AcqTime\" : \"00000000ffffffff\"}}}", &ErrCode);
	bool Result = TestForGetApi(StkWebAppSendObj, "/api/agent/", "Bearer admin manager", ReqObj);
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
	if (ResultCode != 400 || ResObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkObject* ChkObj = new StkObject(L"Code", 4203);
	StkObject* FndObj = ResObj->Contains(ChkObj);
	if (FndObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ChkObj;
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
	if (ResultCode != 400 || ResObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkObject* ChkObj = new StkObject(L"Code", 4201);
	StkObject* FndObj = ResObj->Contains(ChkObj);
	if (FndObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ChkObj;
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
		StkPlSwPrintf(ReqStr, 256, L"{\"Name\":\"%ls\", \"Status\":-980}", HostName[Loop]);
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqStr, &ErrCode);
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", ReqObj, &ResultCode);
		if (ResultCode != 400 || ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4205);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ChkObj;
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
	StkPlPrintf("[OK]\n");

	StkPlPrintf("Get Status/Operation Command (Result checking) ... ");
	if (!ResObj1->Equals(ResObj2)) {
		delete ResObj1;
		delete ResObj2;
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	int ErrCode = 0;
	StkObject *CodeObj = StkObject::CreateObjectFromJson(L"{\"Code\" : 4060}", &ErrCode);
	if (!ResObj1->Contains(CodeObj) || !ResObj2->Contains(CodeObj)) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete CodeObj;

	delete ResObj1;
	delete ResObj2;
	StkPlPrintf("[OK]\n");
}

void TestNewCommand(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("Add new Command ... ");
	{
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"hello\", \"Type\" : 0, \"Script\" : \"echo hello, world!!\", \"ServerFileName\" : \"\", \"AgentFileName\" : \"\"}", &ErrCode);
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/command/", ReqObj, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
	}
	{
		int ResultCode = 0;
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/command/", NULL, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		int ErrCode = 0;
		StkObject* CompObj = StkObject::CreateObjectFromJson(L"\"Command\" : {\"Name\" : \"hello\"}", &ErrCode);
		if (!ResObj->Contains(CompObj)) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete CompObj;
		delete ResObj;
	}
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
	TestNewCommand(StkWebAppSendObj);

	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
