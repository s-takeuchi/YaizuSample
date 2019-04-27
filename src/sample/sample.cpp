#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stksocket/stksocket.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "ApiGetLogInfo.h"
#include "ApiGetServerInfo.h"
#include "ApiPostServerInfo.h"
#include "ApiGetAgentInfo.h"
#include "ApiPostAgentInfo.h"
#include "ApiGetCommandForStatus.h"
#include "dataaccess.h"

void Sample(wchar_t* IpAddr, int Port)
{
	int Ids[32] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

	StkWebApp* Soc = new StkWebApp(Ids, 32, IpAddr, Port);

	ApiGetLogInfo* ApiGetLogInfoObj = new ApiGetLogInfo();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/log/", (StkWebAppExec*)ApiGetLogInfoObj);
	ApiGetServerInfo* ApiGetServerInfoObj = new ApiGetServerInfo();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/", (StkWebAppExec*)ApiGetServerInfoObj);
	ApiGetAgentInfo* ApiGetAgentInfoObj = new ApiGetAgentInfo();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/", (StkWebAppExec*)ApiGetAgentInfoObj);
	ApiPostAgentInfo* ApiPostAgentInfoObj = new ApiPostAgentInfo();
	int Add4 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/", (StkWebAppExec*)ApiPostAgentInfoObj);
	ApiGetCommandForStatus* ApiGetCommandForStatusObj = new ApiGetCommandForStatus();
	int Add5 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/", (StkWebAppExec*)ApiGetCommandForStatusObj);
	ApiPostServerInfo* ApiPostServerInfoObj = new ApiPostServerInfo();
	int Add6 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/", (StkWebAppExec*)ApiPostServerInfoObj);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/log/");
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/");
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/");
	int Del4 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/");
	int Del5 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/");
	int Del6 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/");

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
