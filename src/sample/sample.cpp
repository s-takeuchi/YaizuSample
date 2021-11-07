#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stksocket/stksocket.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "sample.h"
#include "ApiGetServerInfo.h"
#include "ApiPostServerInfo.h"
#include "ApiGetAgentInfo.h"
#include "ApiPostAgentInfo.h"
#include "ApiDeleteAgentInfo.h"
#include "ApiGetCommandForStatus.h"
#include "ApiGetCommand.h"
#include "ApiPostCommand.h"
#include "ApiDeleteCommand.h"
#include "ApiGetCommandForOperation.h"
#include "ApiGetFile.h"
#include "ApiPostFile.h"
#include "ApiGetFileList.h"
#include "ApiDeleteFile.h"
#include "ApiGetCommandResult.h"
#include "ApiGetTimeSeriesData.h"
#include "ApiPostViewSetting.h"
#include "ApiGetViewSetting.h"
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

	// Check status not received from agent
	//
	int PInterval = 0;
	int SaInterval = 0;
	DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);

	wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
	int Status[DA_MAXNUM_OF_AGTRECORDS];
	int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
	int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
	int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
	long long ReqTime[DA_MAXNUM_OF_AGTRECORDS];
	long long AcqTime[DA_MAXNUM_OF_AGTRECORDS];
	long long UpdTime[DA_MAXNUM_OF_AGTRECORDS];
	long long IniTime[DA_MAXNUM_OF_AGTRECORDS];
	long long OpeTime[DA_MAXNUM_OF_AGTRECORDS];
	int ReAgtCount = DataAccess::GetInstance()->GetAgentInfo(AgtName, Status, StatusCmd, OpStatus, OpCmd, ReqTime, AcqTime, UpdTime, IniTime, OpeTime);

	long long CurTime = StkPlGetTime();
	for (int Loop = 0; Loop < ReAgtCount; Loop++) {
		long long ReqTime = DataAccess::GetInstance()->GetAgentInfoForReqTime(AgtName[Loop]);
		if (ReqTime != 0) {
			long long DifTime = CurTime - ReqTime;
			if (DifTime > PInterval + 60) {
				DataAccess::GetInstance()->SetAgentInfoForStatus(AgtName[Loop], -970);
			}
		}
	}

	// Delete expired time series data
	//
	static int WaitCnt = 0;
	if (WaitCnt == 0) { // Works once a four hours
		WaitCnt++;
		int TotalCnt = DataAccess::GetInstance()->GetTotalNumOfTimeSeriesData();
		int Cnt = DataAccess::GetInstance()->DeleteExpiredTimeSeriesData();
		if (Cnt > 0) {
			char Buf[128] = "";
			StkPlSPrintf(Buf, 128, "[Status checker] %d/%d time series data have been deleted.", Cnt, TotalCnt);
			MessageProc::AddLog(Buf, MessageProc::LOG_TYPE_INFO);
		}
	} else if (WaitCnt == 4 * 60 * 4) { // 15sec * 4 * 60min * 4hour
		WaitCnt = 0;
	} else {
		WaitCnt++;
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
	MessageProc::AddJpn(MSG_SERVICESTARTED, L"サービスが開始しました。");
	MessageProc::AddEng(MSG_SERVICESTOPPED, L"Service has stopped.");
	MessageProc::AddJpn(MSG_SERVICESTOPPED, L"サービスが停止しました。");
	MessageProc::AddEng(MSG_SERVICETERMINATED, L"Service has terminated.");
	MessageProc::AddJpn(MSG_SERVICETERMINATED, L"サービスが異常停止しました。");
	MessageProc::AddEng(MSG_AGTDELETE, L"An agent has been deleted.");
	MessageProc::AddJpn(MSG_AGTDELETE, L"エージェントを削除しました。");
	MessageProc::AddEng(MSG_NEWAGTNOTIFIED, L"New agent information has been notified.");
	MessageProc::AddJpn(MSG_NEWAGTNOTIFIED, L"新規にエージェント情報が通知されました。");
	MessageProc::AddEng(MSG_SVRINFOUPDATED, L"Server information has been changed.");
	MessageProc::AddJpn(MSG_SVRINFOUPDATED, L"サーバー情報が更新されました。");
	MessageProc::AddEng(MSG_FILEUPLOADED, L"A file has been uploaded.");
	MessageProc::AddJpn(MSG_FILEUPLOADED, L"ファイルがアップロードされました。");
	MessageProc::AddEng(MSG_FILEDELETED, L"A file has been deleted.");
	MessageProc::AddJpn(MSG_FILEDELETED, L"ファイルが削除されました。");

	// Command
	MessageProc::AddEng(MSG_COMMANDNOTEXIST, L"The specified command does not exist.");
	MessageProc::AddJpn(MSG_COMMANDNOTEXIST, L"指定したコマンドは存在しません。");
	MessageProc::AddEng(MSG_CMDINVALIDTIMEOUT, L"An invalid timeout is specified. Specify in scope of 1 to 28800.");
	MessageProc::AddJpn(MSG_CMDINVALIDTIMEOUT, L"タイムアウト値が不正です。1から28800 の範囲で指定可能です。");
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
	MessageProc::AddEng(MSG_CMDEXCEEDMAXCNT, L"The number of commands can be registered exceeded the limit.");
	MessageProc::AddJpn(MSG_CMDEXCEEDMAXCNT, L"登録可能なコマンド数の上限を超えました。");

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
	MessageProc::AddEng(MSG_FILE_ACCESS_ERROR, L"An error occurred during the target file access.");
	MessageProc::AddJpn(MSG_FILE_ACCESS_ERROR, L"指定したファイルへのアクセス時にエラーが発生しました。");
	MessageProc::AddEng(MSG_FILE_EXCEED_SIZE, L"The specified offset exceeds the file size.");
	MessageProc::AddJpn(MSG_FILE_EXCEED_SIZE, L"指定したオフセットがファイルサイズを超過しました。");
	MessageProc::AddEng(MSG_FILE_INVALID_ORDER, L"Non-continuous data has been received.");
	MessageProc::AddJpn(MSG_FILE_INVALID_ORDER, L"連続していないファイルデータを取得しました。");

	// Result
	MessageProc::AddEng(MSG_RESULT_NOTEXIST, L"The specified command execution result does not exist.");
	MessageProc::AddJpn(MSG_RESULT_NOTEXIST, L"指定したコマンド実行結果は存在しません。");

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

void CommonError_NoElemInRequest(StkObject* TmpObj, const wchar_t* ElemName)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_NO_ELEM_IN_REQUEST), ElemName);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_NO_ELEM_IN_REQUEST), ElemName);
	TmpObj->AppendChildElement(new StkObject(L"Code", MSG_NO_ELEM_IN_REQUEST));
	TmpObj->AppendChildElement(new StkObject(L"MsgEng", MsgEng));
	TmpObj->AppendChildElement(new StkObject(L"MsgJpn", MsgJpn));
}

void CommonError_StringLenError(StkObject* TmpObj, const wchar_t* Name, int MaxLen)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_STRING_LEN_ERROR), Name, MaxLen);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_STRING_LEN_ERROR), Name, MaxLen);
	TmpObj->AppendChildElement(new StkObject(L"Code", MSG_STRING_LEN_ERROR));
	TmpObj->AppendChildElement(new StkObject(L"MsgEng", MsgEng));
	TmpObj->AppendChildElement(new StkObject(L"MsgJpn", MsgJpn));
}

void CommonError_ForbiddenChar(StkObject* TmpObj, const wchar_t* Name)
{
	wchar_t MsgEng[1024] = L"";
	wchar_t MsgJpn[1024] = L"";
	StkPlSwPrintf(MsgEng, 1024, MessageProc::GetMsgEng(MSG_FORBIDDEN_CHAR), Name);
	StkPlSwPrintf(MsgJpn, 1024, MessageProc::GetMsgJpn(MSG_FORBIDDEN_CHAR), Name);
	TmpObj->AppendChildElement(new StkObject(L"Code", MSG_FORBIDDEN_CHAR));
	TmpObj->AppendChildElement(new StkObject(L"MsgEng", MsgEng));
	TmpObj->AppendChildElement(new StkObject(L"MsgJpn", MsgJpn));
}

void EventLogging(wchar_t* MsgEn, wchar_t* MsgJa, int UserId)
{
	StkWebAppUm_AddLogMsg(MsgEn, MsgJa, UserId);
	// Trace logging
	char TraceLogBuf[2048] = "";
	char* TraceLogTmp = StkPlCreateUtf8FromWideChar(MsgEn);
	StkPlSPrintf(TraceLogBuf, 1024, "Event : %s (UserId=%d)", TraceLogTmp, UserId);
	delete TraceLogTmp;
	MessageProc::AddLog(TraceLogBuf, MessageProc::LOG_TYPE_INFO);
}

void Server(wchar_t* IpAddr, int Port, int NumOfWorkerThreads, int ThreadInterval, bool SecureMode, const char* PrivateKey, const char* Certificate)
{
	char LogBuf[1024] = "";
	StkPlSPrintf(LogBuf, 1024, "Number of threads = %d", NumOfWorkerThreads);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

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
	for (int Loop = 0; Loop < NumOfWorkerThreads; Loop++) {
		SetStkThreadInterval(Ids[Loop], ThreadInterval);
	}

	int TargetId[1] = { 100 };
	AddStkThread(100, L"StatusChecker", L"", NULL, NULL, StatusChecker, NULL, NULL);
	StartSpecifiedStkThreads(TargetId, 1);
	MessageProc::AddLog("Status checker has started.", MessageProc::LOG_TYPE_INFO);

	ApiGetCommandForStatus::StopFlag = false;
	ApiGetCommandForOperation::StopFlag = false;

	ApiGetServerInfo* ApiGetServerInfoObj = new ApiGetServerInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/", (StkWebAppExec*)ApiGetServerInfoObj);
	ApiGetAgentInfo* ApiGetAgentInfoObj = new ApiGetAgentInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/", (StkWebAppExec*)ApiGetAgentInfoObj);
	ApiPostAgentInfo* ApiPostAgentInfoObj = new ApiPostAgentInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/", (StkWebAppExec*)ApiPostAgentInfoObj);
	ApiGetCommandForStatus* ApiGetCommandForStatusObj = new ApiGetCommandForStatus();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/$/", (StkWebAppExec*)ApiGetCommandForStatusObj);
	ApiPostServerInfo* ApiPostServerInfoObj = new ApiPostServerInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/", (StkWebAppExec*)ApiPostServerInfoObj);
	ApiGetCommand* ApiGetCommandObj = new ApiGetCommand();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/command/", (StkWebAppExec*)ApiGetCommandObj);
	ApiPostCommand* ApiPostCommandObj = new ApiPostCommand();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/command/", (StkWebAppExec*)ApiPostCommandObj);
	ApiDeleteCommand* ApiDeleteCommandObj = new ApiDeleteCommand();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/command/$/", (StkWebAppExec*)ApiDeleteCommandObj);
	ApiGetCommandForOperation* ApiGetCommandForOperationObj = new ApiGetCommandForOperation();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/opcommand/$/", (StkWebAppExec*)ApiGetCommandForOperationObj);
	ApiGetFile* ApiGetFileObj = new ApiGetFile();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/file/$/$/", (StkWebAppExec*)ApiGetFileObj);
	ApiPostFile* ApiPostFileObj = new ApiPostFile();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/file/", (StkWebAppExec*)ApiPostFileObj);
	ApiGetFileList* ApiGetFileListObj = new ApiGetFileList();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filelist/", (StkWebAppExec*)ApiGetFileListObj);
	ApiDeleteFile* ApiDeleteFileObj = new ApiDeleteFile();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/file/$/", (StkWebAppExec*)ApiDeleteFileObj);
	ApiGetCommandResult* ApiGetCommandResultObj = new ApiGetCommandResult();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/commandresult/", (StkWebAppExec*)ApiGetCommandResultObj);
	ApiGetCommandResult* ApiGetCommandResultWpObj = new ApiGetCommandResult();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/commandresult/$/", (StkWebAppExec*)ApiGetCommandResultWpObj);
	ApiGetTimeSeriesData* ApiGetTimeSeriesDataObj = new ApiGetTimeSeriesData();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/timeseriesdata/$/", (StkWebAppExec*)ApiGetTimeSeriesDataObj);
	ApiPostViewSetting* ApiPostViewSettingObj = new ApiPostViewSetting();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/viewsetting/", (StkWebAppExec*)ApiPostViewSettingObj);
	ApiGetViewSetting* ApiGetViewSettingObj = new ApiGetViewSetting();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/viewsetting/", (StkWebAppExec*)ApiGetViewSettingObj);
	ApiDeleteAgentInfo* ApiDeleteAgentInfoObj = new ApiDeleteAgentInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/agent/$/", (StkWebAppExec*)ApiDeleteAgentInfoObj);

	StkWebAppUm_RegisterApi(Soc);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/server/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/agent/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/agent/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/statuscommand/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/server/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/command/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/command/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/command/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/opcommand/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/file/$/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/file/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filelist/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/file/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/commandresult/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/commandresult/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/timeseriesdata/$/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/viewsetting/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/viewsetting/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_DELETE, L"/api/agent/$/");

	StkWebAppUm_UnregisterApi(Soc);

	ApiGetCommandForStatus::StopFlag = true;
	ApiGetCommandForOperation::StopFlag = true;

	StopSpecifiedStkThreads(TargetId, 1);
	DeleteStkThread(100);
	MessageProc::AddLog("Status checker has ended.", MessageProc::LOG_TYPE_INFO);

	delete Soc;
}

void TerminateService(bool DbClosure)
{
	if (DbClosure) {
		EventLogging(MessageProc::GetMsgEng(MSG_SERVICETERMINATED), MessageProc::GetMsgJpn(MSG_SERVICETERMINATED), -1);
		DataAccess::GetInstance()->StopAutoSave(L"sample.dat");
	}

	// Logging ends
	MessageProc::AddLog("Service terminated", MessageProc::LOG_TYPE_FATAL);
	MessageProc::StopLogging();
	StkPlExit(-1);
}

int main(int Argc, char* Argv[])
{
	char LogBuf[1024] = "";

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

	// Logging starts
	wchar_t LoggingPath[FILENAME_MAX] = L"";
#ifdef WIN32
	StkPlGetFullPathFromFileName(L"sample.log", LoggingPath);
#else
	StkPlWcsCpy(LoggingPath, FILENAME_MAX, L"/var/log/sample.log");
#endif
	MessageProc::StartLogging(LoggingPath);
	MessageProc::AddLog("----------------------------------------", MessageProc::LOG_TYPE_INFO);
	StkPlSPrintf(LogBuf, 1024, "Service started  [Ver=%s, Build=%s %s]", SERVICE_VERSION, __DATE__, __TIME__);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	StkProperties *Prop = new StkProperties();
	
	if (Prop->GetProperties(L"sample.conf") == 0) {

		// servicehost
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			MessageProc::AddLog("servicehost property is not found.", MessageProc::LOG_TYPE_FATAL);
			TerminateService(false);
		}
		StkPlSPrintf(LogBuf, 1024, "servicehost property = %s", IpAddrTmp);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

		// serviceport
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			MessageProc::AddLog("serviceport property is not found.", MessageProc::LOG_TYPE_FATAL);
			TerminateService(false);
		}
		StkPlSPrintf(LogBuf, 1024, "serviceport property = %d", Port);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

		// workerthreads
		if (Prop->GetPropertyInt("workerthreads", &NumOfWorkerThreads) != 0) {
			// How many threads this program needs to be covered.
			// 2 (status acquisition and operation) * 3 (agent / customer) * 30 (customer / server) + 10 (browser access)
			// Considering the limitation of StkSocket
			NumOfWorkerThreads = 190;
		} else {
			StkPlSPrintf(LogBuf, 1024, "workerthreads property = %d", NumOfWorkerThreads);
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}
		if (NumOfWorkerThreads <= 2 || NumOfWorkerThreads > 250) { // Max number of threads is 250
			MessageProc::AddLog("An invalid number of threads is specified.", MessageProc::LOG_TYPE_FATAL);
			TerminateService(false);
		}

		// threadinterval
		if (Prop->GetPropertyInt("threadinterval", &ThreadInterval) != 0) {
			ThreadInterval = 200;
		} else {
			StkPlSPrintf(LogBuf, 1024, "threadinterval property = %d", ThreadInterval);
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}

		// securemode
		if (Prop->GetPropertyStr("securemode", SecureModeStr) == 0) {
			if (StkPlStrCmp(SecureModeStr, "true") == 0) {
				SecureMode = true;
			} else if (StkPlStrCmp(SecureModeStr, "false") == 0) {
				SecureMode = false;
			} else {
				MessageProc::AddLog("An invalid value is specified for 'securemode'.", MessageProc::LOG_TYPE_FATAL);
				TerminateService(false);
			}
			StkPlSPrintf(LogBuf, 1024, "securemode property = %s", SecureMode ? "true" : "false");
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}

		// privatekey
		if (Prop->GetPropertyStr("privatekey", PrivateKey) == 0) {
			StkPlSPrintf(LogBuf, 1024, "privatekey property = %s", PrivateKey);
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}

		// certificate
		if (Prop->GetPropertyStr("certificate", Certificate) == 0) {
			StkPlSPrintf(LogBuf, 1024, "certificate property = %s", Certificate);
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}

		// workdir
		if (Prop->GetPropertyStr("workdir", WorkDir) != 0) {
			wchar_t TmpBuf[FILENAME_MAX] = L"";
			StkPlGetFullPathFromFileName(L".", TmpBuf);
			StkPlConvWideCharToUtf8(WorkDir, 256, TmpBuf);
		} else {
			StkPlSPrintf(LogBuf, 1024, "workdir property = %s", WorkDir);
			MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		}
	} else {
		MessageProc::AddLog("sample.conf is not found.", MessageProc::LOG_TYPE_FATAL);
		TerminateService(false);
	}

	StkWebAppUm_Init();
	int DbStatus = DataAccess::GetInstance()->CreateTables(L"sample.dat");
	if (DbStatus == -1) {
		MessageProc::AddLog("An error occurred during data file access.", MessageProc::LOG_TYPE_FATAL);
		TerminateService(false);
	} else if (DbStatus == -2) {
		MessageProc::AddLog("Data file is broken.", MessageProc::LOG_TYPE_FATAL);
		TerminateService(false);
	}  else if (DbStatus == 0) {
		MessageProc::AddLog("Necessary tables have been newly created.", MessageProc::LOG_TYPE_INFO);
	} else if (DbStatus == 1) {
		MessageProc::AddLog("Existing data file has successfully been loaded.", MessageProc::LOG_TYPE_INFO);
	}
	int DbVer = StkWebAppUm_GetPropertyValueInt(L"DbVersion");
	{
		char LogBuf[1024] = "";
		StkPlSPrintf(LogBuf, 1024, "DB version = %d", DbVer);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
	}
	if (DbVer == -1) {
		StkWebAppUm_CreateTable();
		StkWebAppUm_SetPropertyValueInt(L"DbVersion", 1);
		StkWebAppUm_SetPropertyValueInt(L"MaxResultId", 0);
		StkWebAppUm_SetPropertyValueInt(L"MaxAgentId", 0);
		// For view setting
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d1", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d2", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d3", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d4", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d5", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d6", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d7", L"");
		StkWebAppUm_SetPropertyValueWStr(L"ViewSetting_d8", L"");
		DbVer = 1;
	}

	EventLogging(MessageProc::GetMsgEng(MSG_SERVICESTARTED), MessageProc::GetMsgJpn(MSG_SERVICESTARTED), -1);
	wchar_t* IpAddr = StkPlCreateWideCharFromUtf8(IpAddrTmp);
	wchar_t* WorkDirWc = StkPlCreateWideCharFromUtf8(WorkDir);
	StkPlWcsCpy(Global::Global_WorkDirPath, FILENAME_MAX, WorkDirWc);

	// Launch threads start
	Server(IpAddr, Port, NumOfWorkerThreads, ThreadInterval, SecureMode, PrivateKey, Certificate);
	// Launch threads end

	delete IpAddr;
	delete WorkDirWc;
	EventLogging(MessageProc::GetMsgEng(MSG_SERVICESTOPPED), MessageProc::GetMsgJpn(MSG_SERVICESTOPPED), -1);

	DataAccess::GetInstance()->StopAutoSave(L"sample.dat");

	// Logging ends
	MessageProc::AddLog("Service ended", MessageProc::LOG_TYPE_INFO);
	MessageProc::StopLogging();

	return 0;
}
