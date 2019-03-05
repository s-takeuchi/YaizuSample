#include "../../../YaizuComLib/src/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stksocket/stksocket.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "sample_elem1.h"
#include "sample_elem2.h"
#include "sample_elem3.h"

void Sample(wchar_t* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 3, IpAddr, Port);

	Sample_Elem1* Test1Hndl = new Sample_Elem1();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/bbb/", (StkWebAppExec*)Test1Hndl);
	Sample_Elem2* Test2Hndl = new Sample_Elem2();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/xxx/", (StkWebAppExec*)Test2Hndl);
	Sample_Elem3* Test3Hndl = new Sample_Elem3();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/bbb/$/", (StkWebAppExec*)Test3Hndl);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/bbb/");
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/aaa/xxx/");
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/bbb/$/");

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
		StkPlPrintf("stkwebapp.conf is not found.\r\n");
		return -1;
	}

	wchar_t* IpAddr = StkPlCreateWideCharFromUtf8(IpAddrTmp);
	Sample(IpAddr, Port);
	delete IpAddr;

	return 0;
}
