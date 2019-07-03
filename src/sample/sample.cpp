#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
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
#include "ApiGetCommand.h"
#include "ApiPostCommand.h"
#include "ApiDeleteCommand.h"
#include "ApiGetCommandForOperation.h"
#include "ApiGetFile.h"
#include "ApiPostFile.h"
#include "ApiGetLanguage.h"
#include "dataaccess.h"
#include "MessageCode.h"

int StatusChecker(int Id)
{
	StkPlSleepMs(15000);

	int PInterval = 0;
	int SaInterval = 0;
	wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH] = L"";
	DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval, BucketPath);

	wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	int Status[DA_MAXNUM_OF_AGTRECORDS];
	int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
	int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
	int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
	wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
	int ReAgtCount = DataAccess::GetInstance()->GetAgentInfo(AgtName, Status, StatusCmd, OpStatus, OpCmd, TimeUtc, TimeLocal, UdTimeUtc, UdTimeLocal);

	for (int Loop = 0; Loop < ReAgtCount; Loop++) {
		long long ReqTime = DataAccess::GetInstance()->GetAgentInfoForReqTime(AgtName[Loop]);
		if (ReqTime != 0) {
			long long CurTime = StkPlGetTime();
			long long DifTime = CurTime - ReqTime;
			if (DifTime > PInterval + 60) {
				DataAccess::GetInstance()->SetAgentInfoForStatus(AgtName[Loop], -993);
			}
		}
	}

	return 0;
}

void InitMessageResource()
{
	MessageProc::ClearAllMsg();
	MessageProc::AddEng(MSG_COMDELETE, L"A command has been deleted.");
	MessageProc::AddJpn(MSG_COMDELETE, L"コマンドを削除しました。");
	MessageProc::AddEng(MSG_COMADD, L"A command has been added.");
	MessageProc::AddJpn(MSG_COMADD, L"コマンドを追加しました。");
	MessageProc::AddEng(MSG_COMMODIFY, L"A command has been modified.");
	MessageProc::AddJpn(MSG_COMMODIFY, L"コマンドを変更しました。");
	MessageProc::AddEng(MSG_CMDSTATUSACQCHGD, L"Command for status acquisition has been changed.");
	MessageProc::AddJpn(MSG_CMDSTATUSACQCHGD, L"状態取得コマンドが変更されました。");
	MessageProc::AddEng(MSG_CMDOPSTARTED, L"Command for operation has started.");
	MessageProc::AddJpn(MSG_CMDOPSTARTED, L"操作コマンドが開始しました。");
	MessageProc::AddEng(MSG_CMDOPENDED, L"Command for operation has ended.");
	MessageProc::AddJpn(MSG_CMDOPENDED, L"操作コマンドが終了しました。");
	MessageProc::AddEng(MSG_SERVICESTARTED, L"Service has started.");
	MessageProc::AddJpn(MSG_SERVICESTARTED, L"サービスが開始されました。");
	MessageProc::AddEng(MSG_SERVICESTOPPED, L"Service has stopped.");
	MessageProc::AddJpn(MSG_SERVICESTOPPED, L"サービスが停止しました。");
	MessageProc::AddEng(MSG_NEWAGTNOTIFIED, L"New agent information has been notified.");
	MessageProc::AddJpn(MSG_NEWAGTNOTIFIED, L"新規にエージェント情報が通知されました。");
	MessageProc::AddEng(MSG_SVRINFOUPDATED, L"Server information has been changed.");
	MessageProc::AddJpn(MSG_SVRINFOUPDATED, L"サーバー情報が更新されました。");
	MessageProc::AddEng(MSG_COMMANDNOTEXIST, L"The specified command does not exist.");
	MessageProc::AddJpn(MSG_COMMANDNOTEXIST, L"指定したコマンドは存在しません。");
	MessageProc::AddEng(MSG_NOREQUEST, L"No request has been presented.");
	MessageProc::AddJpn(MSG_NOREQUEST, L"リクエストが存在しません。");
	MessageProc::AddEng(MSG_NOCOMMANDREQUEST, L"No \"Command\" element has been presented in the request.");
	MessageProc::AddJpn(MSG_NOCOMMANDREQUEST, L"リクエストに\"Command\"要素が存在しません。");
	MessageProc::AddEng(MSG_NOCOMMANDNAME, L"No command name has been specified.");
	MessageProc::AddJpn(MSG_NOCOMMANDNAME, L"コマンド名が指定されていません。");
	MessageProc::AddEng(MSG_COMNAMELENGTHERR, L"Length of command name exceeds the limitation. Maximum 31 characters can be specified as a command name.");
	MessageProc::AddJpn(MSG_COMNAMELENGTHERR, L"コマンド名の長さが最大を超えました。コマンド名として最大31文字まで指定可能です。");
	MessageProc::AddEng(MSG_COMSCRIPTLENERR, L"Length of script exceeds the limitation. Maximum 4096 characters can be specified as a script.");
	MessageProc::AddJpn(MSG_COMSCRIPTLENERR, L"スクリプトの長さが最大を超えました。スクリプトとして最大4096文字まで指定可能です。");
	MessageProc::AddEng(MSG_FILENAMELENERR, L"Length of file name exceeds the limitation. Maximum 256 characters can be specified as a file name.");
	MessageProc::AddJpn(MSG_FILENAMELENERR, L"ファイル名の長さが最大を超えました。ファイル名として最大256文字まで指定可能です。");
	MessageProc::AddEng(MSG_FILENAMEFBDNCHAR, L"File name includes forbidden character(s) for use. [\\ / : * ? \" < > | & %] and space");
	MessageProc::AddJpn(MSG_FILENAMEFBDNCHAR, L"ファイル名に使用禁止文字が含まれています。[\\ / : * ? \" < > | & %] and space");
}

void Sample(wchar_t* IpAddr, int Port)
{
	int Ids[32] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

	StkWebApp* Soc = new StkWebApp(Ids, 32, IpAddr, Port);
	Soc->SetSendBufSize(5000000);
	Soc->SetRecvBufSize(5000000);

	int TargetId[1] = { 100 };
	AddStkThread(100, L"StatusChecker", L"", NULL, NULL, StatusChecker, NULL, NULL);
	StartSpecifiedStkThreads(TargetId, 1);

	ApiGetCommandForStatus::StopFlag = false;
	ApiGetCommandForOperation::StopFlag = false;

	ApiGetLogInfo* ApiGetLogInfoObj = new ApiGetLogInfo();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/log/", (StkWebAppExec*)ApiGetLogInfoObj);
	ApiGetServerInfo* ApiGetServerInfoObj = new ApiGetServerInfo();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/", (StkWebAppExec*)ApiGetServerInfoObj);
	ApiGetAgentInfo* ApiGetAgentInfoObj = new ApiGetAgentInfo();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/", (StkWebAppExec*)ApiGetAgentInfoObj);
	ApiPostAgentInfo* ApiPostAgentInfoObj = new ApiPostAgentInfo();
	int Add4 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/", (StkWebAppExec*)ApiPostAgentInfoObj);
	ApiGetCommandForStatus* ApiGetCommandForStatusObj = new ApiGetCommandForStatus();
	int Add5 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/$/", (StkWebAppExec*)ApiGetCommandForStatusObj);
	ApiPostServerInfo* ApiPostServerInfoObj = new ApiPostServerInfo();
	int Add6 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/", (StkWebAppExec*)ApiPostServerInfoObj);
	ApiGetCommand* ApiGetCommandObj = new ApiGetCommand();
	int Add7 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/command/", (StkWebAppExec*)ApiGetCommandObj);
	ApiPostCommand* ApiPostCommandObj = new ApiPostCommand();
	int Add8 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/command/", (StkWebAppExec*)ApiPostCommandObj);
	ApiDeleteCommand* ApiDeleteCommandObj = new ApiDeleteCommand();
	int Add9 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/command/$/", (StkWebAppExec*)ApiDeleteCommandObj);
	ApiGetCommandForOperation* ApiGetCommandForOperationObj = new ApiGetCommandForOperation();
	int Add10 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/opcommand/$/", (StkWebAppExec*)ApiGetCommandForOperationObj);
	ApiGetFile* ApiGetFileObj = new ApiGetFile();
	int Add11 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/file/$/$/", (StkWebAppExec*)ApiGetFileObj);
	ApiPostFile* ApiPostFileObj = new ApiPostFile();
	int Add12 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/file/", (StkWebAppExec*)ApiPostFileObj);
	ApiGetLanguage* ApiGetLanguageObj = new ApiGetLanguage();
	int Add13 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/language/", (StkWebAppExec*)ApiGetLanguageObj);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/log/");
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/");
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/");
	int Del4 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/");
	int Del5 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/$/");
	int Del6 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/");
	int Del7 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/command/");
	int Del8 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/command/");
	int Del9 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/command/$/");
	int Del10 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/opcommand/$/");
	int Del11 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/file/$/$/");
	int Del12 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/file/");
	int Del13 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/language/");

	ApiGetCommandForStatus::StopFlag = true;
	ApiGetCommandForOperation::StopFlag = true;

	StopSpecifiedStkThreads(TargetId, 1);
	DeleteStkThread(100);

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	InitMessageResource();

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
	DataAccess::GetInstance()->AddLogMsg(MessageProc::GetMsgEng(MSG_SERVICESTARTED), MessageProc::GetMsgJpn(MSG_SERVICESTARTED));
	wchar_t* IpAddr = StkPlCreateWideCharFromUtf8(IpAddrTmp);
	Sample(IpAddr, Port);
	delete IpAddr;
	DataAccess::GetInstance()->AddLogMsg(MessageProc::GetMsgEng(MSG_SERVICESTOPPED), MessageProc::GetMsgJpn(MSG_SERVICESTOPPED));
	DataAccess::GetInstance()->StopAutoSave(L"sample.dat");

	return 0;
}
