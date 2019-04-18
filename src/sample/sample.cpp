#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stksocket/stksocket.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "ApiGetLogInfo.h"
#include "sample_elem2.h"
#include "sample_elem3.h"
#include "ApiPostAgentInfo.h"
#include "dataaccess.h"

void Sample(wchar_t* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 3, IpAddr, Port);

	ApiGetLogInfo* ApiGetLogInfoObj = new ApiGetLogInfo();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/log/", (StkWebAppExec*)ApiGetLogInfoObj);
	ApiGetServerInfo* ApiGetServerInfoObj = new ApiGetServerInfo();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/xxx/", (StkWebAppExec*)ApiGetServerInfoObj);
	Sample_Elem3* Test3Hndl = new Sample_Elem3();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/bbb/$/", (StkWebAppExec*)Test3Hndl);
	ApiPostAgentInfo* ApiPostAgentInfoObj = new ApiPostAgentInfo();
	int Add4 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/", (StkWebAppExec*)ApiPostAgentInfoObj);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/bbb/");
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/xxx/");
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/bbb/$/");
	int Del4 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/");

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	char IpAddrTmp[256];
	int Port;
	StkProperties *Prop = new StkProperties();
	
	if (Prop->GetProperties(L"sample.conf") == 0) {
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			StkPlPrintf("servicehost property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("servicehost property = %s\r\n", IpAddrTmp);
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			StkPlPrintf("serviceport property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("serviceport property = %d\r\n", Port);
	} else {
		StkPlPrintf("sample.conf is not found.\r\n");
		return -1;
	}

	DataAccess::GetInstance()->CreateTables(L"sample.dat");
	DataAccess::GetInstance()->AddLogMsg(L"Service started.");
	wchar_t* IpAddr = StkPlCreateWideCharFromUtf8(IpAddrTmp);
	Sample(IpAddr, Port);
	delete IpAddr;
	DataAccess::GetInstance()->AddLogMsg(L"Service stopped.");
	DataAccess::GetInstance()->StopAutoSave(L"sample.dat");

	return 0;
}
