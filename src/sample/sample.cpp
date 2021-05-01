#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stksocket/stksocket.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "sample.h"
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
#include "ApiGetFileList.h"
#include "ApiDeleteFile.h"
#include "dataaccess.h"

wchar_t Global::Global_WorkDirPath[FILENAME_MAX];

void GetFullPathFromFileName(wchar_t FullPath[FILENAME_MAX], const wchar_t FileName[FILENAME_MAX])
{
	StkPlWcsCpy(FullPath, FILENAME_MAX, Global::Global_WorkDirPath);
	int FullPathLen = (int)StkPlWcsLen(FullPath);
	if (FullPathLen >= 1) {
#ifdef WIN32
		if (FullPath[FullPathLen - 1] != L'\\') {
			StkPlWcsCat(FullPath, FILENAME_MAX, L"\\");
		}
#else
		if (FullPath[FullPathLen - 1] != L'/') {
			StkPlWcsCat(FullPath, FILENAME_MAX, L"/");
		}
#endif
	}
	StkPlWcsCat(FullPath, FILENAME_MAX, FileName);
}

int StatusChecker(int Id)
{
	StkPlSleepMs(15000);

	int PInterval = 0;
	int SaInterval = 0;
	DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);

	wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	int Status[DA_MAXNUM_OF_AGTRECORDS];
	int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
	int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
	int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
	long long AcqTime[DA_MAXNUM_OF_AGTRECORDS];
	long long UpdTime[DA_MAXNUM_OF_AGTRECORDS];
	int ReAgtCount = DataAccess::GetInstance()->GetAgentInfo(AgtName, Status, StatusCmd, OpStatus, OpCmd, AcqTime, UpdTime);

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

	// Audit log
	MessageProc::AddEng(MSG_COMDELETE, L"A command has been deleted.");
	MessageProc::AddJpn(MSG_COMDELETE, L"コマンドを削除しました。");
	MessageProc::AddEng(MSG_COMADD, L"A command has been added.");
	MessageProc::AddJpn(MSG_COMADD, L"コマンドを追加しました。");
	MessageProc::AddEng(MSG_COMMODIFY, L"A command has been modified.");
	MessageProc::AddJpn(MSG_COMMODIFY, L"コマンドを変更しました。");
	MessageProc::AddEng(MSG_CMDOPERATIONCHGD, L"Command for operation has been changed.");
	MessageProc::AddJpn(MSG_CMDOPERATIONCHGD, L"操作コマンドが変更されました。");
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
	
	// Command
	MessageProc::AddEng(MSG_COMMANDNOTEXIST, L"The specified command does not exist.");
	MessageProc::AddJpn(MSG_COMMANDNOTEXIST, L"指定したコマンドは存在しません。");
	MessageProc::AddEng(MSG_NOCOMMANDNAME, L"No command name has been specified.");
	MessageProc::AddJpn(MSG_NOCOMMANDNAME, L"コマンド名が指定されていません。");
	MessageProc::AddEng(MSG_COMNAMELENGTHERR, L"Length of command name exceeds the limitation. Maximum 31 characters can be specified as a command name.");
	MessageProc::AddJpn(MSG_COMNAMELENGTHERR, L"コマンド名の長さが最大を超えました。コマンド名として最大31文字まで指定可能です。");
	MessageProc::AddEng(MSG_COMSCRIPTLENERR, L"Length of script exceeds the limitation. Maximum 4095 characters can be specified as a script.");
	MessageProc::AddJpn(MSG_COMSCRIPTLENERR, L"スクリプトの長さが最大を超えました。スクリプトとして最大4095文字まで指定可能です。");
	MessageProc::AddEng(MSG_FILENAMELENERR, L"Length of file name exceeds the limitation. Maximum 255 characters can be specified as a file name.");
	MessageProc::AddJpn(MSG_FILENAMELENERR, L"ファイル名の長さが最大を超えました。ファイル名として最大255文字まで指定可能です。");
	MessageProc::AddEng(MSG_FILENAMEFBDNCHAR, L"File name includes forbidden character(s) for use. [\\ / : * ? \" < > | & %] and space");
	MessageProc::AddJpn(MSG_FILENAMEFBDNCHAR, L"ファイル名に使用禁止文字が含まれています。[\\ / : * ? \" < > | & %] およびスペース");
	MessageProc::AddEng(MSG_DUPCMDNAME, L"The command which has same command name exists. Change the command name.");
	MessageProc::AddJpn(MSG_DUPCMDNAME, L"すでに同名のコマンドが存在します。コマンド名を見直してください。");
	
	// Server Info
	MessageProc::AddEng(MSG_NOPOLLINGINTVL, L"No \"PollingInterval\" element has been presented in the request.");
	MessageProc::AddJpn(MSG_NOPOLLINGINTVL, L"リクエストに\"PollingInterval\"要素が存在しません。");
	MessageProc::AddEng(MSG_NOSAINTVL, L"No \"StatusAcquisitionInterval\" element has been presented in the request.");
	MessageProc::AddJpn(MSG_NOSAINTVL, L"リクエストに\"StatusAcquisitionInterval\"要素が存在しません。");
	MessageProc::AddEng(MSG_INVALIDPOINTVL, L"The value of \"PollingInterval\" is out of permission. Specify the value in the range of 30-900.");
	MessageProc::AddJpn(MSG_INVALIDPOINTVL, L"\"PollingInterval\"の値が許可された範囲を超えました。30-900の範囲で指定してください。");
	MessageProc::AddEng(MSG_INVALIDSVINTVL, L"The value of \"StatusAcquisitionInterval\" is out of permission. Specify the value in the range of 300-3600.");
	MessageProc::AddJpn(MSG_INVALIDSVINTVL, L"\"StatusAcquisitionInterval\"の値が許可された範囲を超えました。300-3600の範囲で指定してください。");
	
	// Agent Info
	MessageProc::AddEng(MSG_AGENTINFO_NOT_FOUND, L"The specified agent information is not found.");
	MessageProc::AddJpn(MSG_AGENTINFO_NOT_FOUND, L"指定したエージェント情報が見つかりません。");

	// File
	MessageProc::AddEng(MSG_FILE_NOTEXIST, L"The specified agent information is not found.");
	MessageProc::AddJpn(MSG_FILE_NOTEXIST, L"指定したエージェント情報が見つかりません。");
	MessageProc::AddEng(MSG_FILE_EXCEED_SIZE, L"The specified offset exceeds the file size.");
	MessageProc::AddJpn(MSG_FILE_EXCEED_SIZE, L"指定したオフセットがファイルサイズを超過しました。");

	// Common error message
	MessageProc::AddEng(MSG_NO_ELEM_IN_REQUEST, L"No \"%ls\" element has been presented in the request.");
	MessageProc::AddJpn(MSG_NO_ELEM_IN_REQUEST, L"リクエストに\"%ls\"要素が存在しません。");
	MessageProc::AddEng(MSG_STRING_LEN_ERROR, L"Length of %ls exceeds the limitation. Maximum %d characters can be specified.");
	MessageProc::AddJpn(MSG_STRING_LEN_ERROR, L"%lsの長さが最大を超えました。最大%d文字まで指定可能です。");
	MessageProc::AddEng(MSG_NOREQUEST, L"No request has been presented.");
	MessageProc::AddJpn(MSG_NOREQUEST, L"リクエストが存在しません。");
	MessageProc::AddEng(MSG_NO_EXEC_RIGHT, L"User does not have execute right for this request.");
	MessageProc::AddJpn(MSG_NO_EXEC_RIGHT, L"ユーザーは，このリクエストの実行権限を持っていません。");
	MessageProc::AddEng(MSG_FORBIDDEN_CHAR, L"\"%ls\" includes forbidden character(s).");
	MessageProc::AddJpn(MSG_FORBIDDEN_CHAR, L"\"%ls\"は使用禁止文字が含まれています。");
	MessageProc::AddEng(MSG_COMMON_AUTH_ERROR, L"Authentication error occurred.");
	MessageProc::AddJpn(MSG_COMMON_AUTH_ERROR, L"認証エラーが発生しました。");

	MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_ENGLISH);
}

void Server(wchar_t* IpAddr, int Port, int NumOfWorkerThreads, int ThreadInterval, bool SecureMode, const char* PrivateKey, const char* Certificate)
{
	int Ids[MAX_NUM_OF_STKTHREADS];
	for (int Loop = 0; Loop < NumOfWorkerThreads; Loop++) {
		Ids[Loop] = 1000 + Loop;
	}

	StkWebApp* Soc = NULL;
	if (SecureMode) {
		Soc = new StkWebApp(Ids, NumOfWorkerThreads, IpAddr, Port, PrivateKey, Certificate);
	} else {
		Soc = new StkWebApp(Ids, NumOfWorkerThreads, IpAddr, Port);
	}
	Soc->SetSendBufSize(5000000);
	Soc->SetRecvBufSize(5000000);
	for (int Loop = 0; Loop < NumOfWorkerThreads; Loop++) {
		SetStkThreadInterval(Ids[Loop], ThreadInterval);
	}

	int TargetId[1] = { 100 };
	AddStkThread(100, L"StatusChecker", L"", NULL, NULL, StatusChecker, NULL, NULL);
	StartSpecifiedStkThreads(TargetId, 1);

	ApiGetCommandForStatus::StopFlag = false;
	ApiGetCommandForOperation::StopFlag = false;

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
	ApiGetFileList* ApiGetFileListObj = new ApiGetFileList();
	int Add13 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filelist/", (StkWebAppExec*)ApiGetFileListObj);
	ApiDeleteFile* ApiDeleteFileObj = new ApiDeleteFile();
	int Add14 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/file/$/", (StkWebAppExec*)ApiDeleteFileObj);
	StkWebAppUm_RegisterApi(Soc);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

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
	int Del13 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filelist/");
	int Del14 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/file/$/");
	StkWebAppUm_UnregisterApi(Soc);

	ApiGetCommandForStatus::StopFlag = true;
	ApiGetCommandForOperation::StopFlag = true;

	StopSpecifiedStkThreads(TargetId, 1);
	DeleteStkThread(100);

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	InitMessageResource();

	char IpAddrTmp[256] = "";
	int Port = 0;
	int NumOfWorkerThreads = 0;
	int ThreadInterval = 0;
	bool SecureMode = false;
	char SecureModeStr[256] = "";
	char PrivateKey[256] = "";
	char Certificate[256] = "";
	char WorkDir[256] = "";

	StkProperties *Prop = new StkProperties();
	
	if (Prop->GetProperties(L"sample.conf") == 0) {
		// servicehost
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			StkPlPrintf("servicehost property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("servicehost property = %s\r\n", IpAddrTmp);

		// serviceport
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			StkPlPrintf("serviceport property is not found.\r\n");
			return -1;
		}
		StkPlPrintf("serviceport property = %d\r\n", Port);

		// workerthreads
		if (Prop->GetPropertyInt("workerthreads", &NumOfWorkerThreads) != 0) {
			// How many threads this program needs to be covered.
			// 2 (status acquisition and operation) * 3 (agent / customer) * 40 (customer / server) + 10 (browser access)
			// Considering the limitation of StkSocket
			NumOfWorkerThreads = 250;
		}
		if (NumOfWorkerThreads <= 2 || NumOfWorkerThreads > 250) { // Max number of threads is 250
			StkPlPrintf("An invalid number of threads is specified.\r\n");
			return -1;
		}
		StkPlPrintf("workerthreads property = %d\r\n", NumOfWorkerThreads);

		// threadinterval
		if (Prop->GetPropertyInt("threadinterval", &ThreadInterval) != 0) {
			ThreadInterval = 200;
		}
		StkPlPrintf("threadinterval property = %d\r\n", ThreadInterval);

		// securemode
		if (Prop->GetPropertyStr("securemode", SecureModeStr) == 0) {
			if (StkPlStrCmp(SecureModeStr, "true") == 0) {
				SecureMode = true;
			}
			if (StkPlStrCmp(SecureModeStr, "false") == 0) {
				SecureMode = false;
			}
		}
		StkPlPrintf("securemode property = %s\r\n", SecureMode? "true" : "false");

		// privatekey
		if (Prop->GetPropertyStr("privatekey", PrivateKey) != 0) {
			//
		}
		StkPlPrintf("privatekey property = %s\r\n", PrivateKey);

		// certificate
		if (Prop->GetPropertyStr("certificate", Certificate) != 0) {
			//
		}
		StkPlPrintf("certificate property = %s\r\n", Certificate);

		// workdir
		if (Prop->GetPropertyStr("workdir", WorkDir) != 0) {
			//
		}
		StkPlPrintf("workdir property = %s\r\n", WorkDir);
	} else {
		StkPlPrintf("sample.conf is not found.\r\n");
		return -1;
	}

	DataAccess::GetInstance()->CreateTables(L"sample.dat");
	StkWebAppUm_Init();
	StkWebAppUm_CreateTable();
	StkWebAppUm_SetPropertyValueInt(L"DbVersion", 1);

	StkWebAppUm_AddLogMsg(MessageProc::GetMsgEng(MSG_SERVICESTARTED), MessageProc::GetMsgJpn(MSG_SERVICESTARTED), -1);
	wchar_t* IpAddr = StkPlCreateWideCharFromUtf8(IpAddrTmp);
	wchar_t* WorkDirWc = StkPlCreateWideCharFromUtf8(WorkDir);
	StkPlWcsCpy(Global::Global_WorkDirPath, FILENAME_MAX, WorkDirWc);

	// Launch threads start
	Server(IpAddr, Port, NumOfWorkerThreads, ThreadInterval, SecureMode, PrivateKey, Certificate);
	// Launch threads end

	delete IpAddr;
	delete WorkDirWc;
	StkWebAppUm_AddLogMsg(MessageProc::GetMsgEng(MSG_SERVICESTOPPED), MessageProc::GetMsgJpn(MSG_SERVICESTOPPED), -1);

	DataAccess::GetInstance()->StopAutoSave(L"sample.dat");

	return 0;
}
