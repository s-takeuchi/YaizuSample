#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

int main(int argvc, char* argv[])
{
	StkObject* NewObj = new StkObject(L"");
	NewObj->AppendChildElement(new StkObject(L"aaa", 1));
	StkWebAppSend SendObj(1, L"localhost", 8081);
	int Result = 0;
	StkObject* ResObj = SendObj.SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NewObj, &Result);
	return 0;
}
