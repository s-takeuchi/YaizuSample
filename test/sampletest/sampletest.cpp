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
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* ResUserObj = ResObj->GetFirstChildElement();
		if (ResUserObj == NULL || StkPlWcsCmp(ResUserObj->GetName(), L"User") != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\r\n");
	}

	{
		StkPlPrintf("GetUser(dummy) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer dummy dummy");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResultCode != 403) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\r\n");
	}
}

void TestPostOperationStop(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("PostOperationStop ... ");
	int ResultCode = 0;
	StkObject* ReqObj = new StkObject(L"");
	ReqObj->AppendChildElement(new StkObject(L"Operation", L"Stop"));
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
	StkPlPrintf("[OK]\r\n");
}

int main(int Argc, char* Argv[])
{
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 10009);
	TestGetUser(StkWebAppSendObj);
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
