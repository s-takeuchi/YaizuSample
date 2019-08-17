#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkdata/stkdata.h"
#include "../../../YaizuComLib/src/stkdata/stkdataapi.h"

DataAccess* DataAccess::ThisInstance;

DataAccess::DataAccess()
{
}

DataAccess::~DataAccess()
{
}

// Get this instance
DataAccess* DataAccess::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new DataAccess();
		Init = 0;
	}
	return ThisInstance;
}

// Stops AutoSave function and save the latest data
// Return : always zero returned
int DataAccess::StopAutoSave(const wchar_t* DataFileName)
{
	wchar_t Buf[FILENAME_MAX];
#ifdef WIN32
	StkPlGetFullPathFromFileName(DataFileName, Buf);
#else
	StkPlWcsCpy(Buf, FILENAME_MAX, L"/etc/");
	StkPlWcsCat(Buf, FILENAME_MAX, DataFileName);
#endif
	AutoSave(Buf, 30, false);
	LockAllTable(2);
	SaveData(Buf);
	UnlockAllTable();
	return 0;
}

// Create tables for CmdFreak
// DataFileName [in] : data file name which you want to preserve.
// Return : [0:Success, -1:Failed]
int DataAccess::CreateTables(const wchar_t* DataFileName)
{
	// Make full path name and call AutoSave
	wchar_t Buf[FILENAME_MAX];
#ifdef WIN32
	StkPlGetFullPathFromFileName(DataFileName, Buf);
#else
	StkPlWcsCpy(Buf, FILENAME_MAX, L"/etc/");
	StkPlWcsCat(Buf, FILENAME_MAX, DataFileName);
#endif
	AutoSave(Buf, 30, true);

	if (StkPlGetFileSize(Buf) == 0) {
		LockAllTable(2);
		// Log table
		{
			ColumnDefInt ColDefLogId(L"Id");
			ColumnDefWStr ColDefLogTimeUtc(L"TimeUtc", DA_MAXLEN_OF_TIME);
			ColumnDefWStr ColDefLogTimeLocal(L"TimeLocal", DA_MAXLEN_OF_TIME);
			ColumnDefWStr ColDefLogMsg(L"Message", DA_MAXLEN_OF_LOGMSG);
			ColumnDefWStr ColDefLogMsgJa(L"MessageJa", DA_MAXLEN_OF_LOGMSG);
			TableDef TabDefLog(L"Log", DA_MAXNUM_OF_LOGRECORDS);
			TabDefLog.AddColumnDef(&ColDefLogId);
			TabDefLog.AddColumnDef(&ColDefLogTimeUtc);
			TabDefLog.AddColumnDef(&ColDefLogTimeLocal);
			TabDefLog.AddColumnDef(&ColDefLogMsg);
			TabDefLog.AddColumnDef(&ColDefLogMsgJa);
			if (CreateTable(&TabDefLog) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Agent info table
		{
			ColumnDefWStr ColDefAgtName(L"Name", DA_MAXLEN_OF_AGTNAME);
			ColumnDefInt  ColDefAgtStatus(L"Status");
			ColumnDefInt  ColDefAgtStatusCmd(L"StatusCmd");
			ColumnDefInt  ColDefAgtOpStatus(L"OpStatus");
			ColumnDefInt  ColDefAgtOpCmd(L"OpCmd");
			ColumnDefBin  ColDefAgtReqTime(L"ReqTime", DA_MAXLEN_OF_REQTIME);
			ColumnDefWStr ColDefAgtTimeUtc(L"TimeUtc", DA_MAXLEN_OF_TIME);
			ColumnDefWStr ColDefAgtTimeLocal(L"TimeLocal", DA_MAXLEN_OF_TIME);
			ColumnDefWStr ColDefAgtUdTimeUtc(L"UdTimeUtc", DA_MAXLEN_OF_TIME);
			ColumnDefWStr ColDefAgUdtTimeLocal(L"UdTimeLocal", DA_MAXLEN_OF_TIME);
			TableDef TabDefAgtInfo(L"AgentInfo", DA_MAXNUM_OF_AGTRECORDS);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtName);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatusCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtReqTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtTimeUtc);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtTimeLocal);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtUdTimeUtc);
			TabDefAgtInfo.AddColumnDef(&ColDefAgUdtTimeLocal);
			if (CreateTable(&TabDefAgtInfo) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Server info table
		{
			ColumnDefInt ColDefSvrId(L"Id");
			ColumnDefInt ColDefSvrPi(L"PInterval");
			ColumnDefInt ColDefSvrSai(L"SaInterval");
			ColumnDefInt ColDefSvrMaxComId(L"MaxCommandId");
			ColumnDefWStr ColDefSvrBucketPath(L"BucketPath", DA_MAXLEN_OF_BUCKETPATH);
			TableDef TabDefSvrInfo(L"ServerInfo", 1);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrId);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrPi);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrSai);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrMaxComId);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrBucketPath);
			if (CreateTable(&TabDefSvrInfo) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Command table
		{
			ColumnDefInt ColDefComId(L"Id");
			ColumnDefWStr ColDefComName(L"Name", DA_MAXLEN_OF_CMDNAME);
			ColumnDefInt ColDefComType(L"Type");
			ColumnDefBin ColDefComScript(L"Script", DA_MAXLEN_OF_CMDSCRIPT);
			ColumnDefWStr ColDefComServerFileName(L"ServerFileName", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComAgentFileName(L"AgentFileName", DA_MAXLEN_OF_AGENTFILENAME);
			TableDef TabDefCommand(L"Command", DA_MAXNUM_OF_CMDRECORDS);
			TabDefCommand.AddColumnDef(&ColDefComId);
			TabDefCommand.AddColumnDef(&ColDefComName);
			TabDefCommand.AddColumnDef(&ColDefComType);
			TabDefCommand.AddColumnDef(&ColDefComScript);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName);
			if (CreateTable(&TabDefCommand) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// User table
		{
			ColumnDefWStr ColDefUserName(L"Name", DA_MAXLEN_OF_USERNAME);
			ColumnDefWStr ColDefUserPassword(L"Password", DA_MAXLEN_OF_PASSWORD);
			ColumnDefInt ColDefUserRole(L"Role");
			ColumnDefWStr ColDefUserUrl(L"Url", DA_MAXLEN_OF_TARGETURL);
			TableDef TabDefCommand(L"User", DA_MAXNUM_OF_USERRECORDS);
			TabDefCommand.AddColumnDef(&ColDefUserName);
			TabDefCommand.AddColumnDef(&ColDefUserPassword);
			TabDefCommand.AddColumnDef(&ColDefUserRole);
			TabDefCommand.AddColumnDef(&ColDefUserUrl);
			if (CreateTable(&TabDefCommand) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		UnlockAllTable();

		// For server info table
		{
			ColumnData *ColDatSvr[5];
			ColDatSvr[0] = new ColumnDataInt(L"Id", 0);
			ColDatSvr[1] = new ColumnDataInt(L"PInterval", 300);
			ColDatSvr[2] = new ColumnDataInt(L"SaInterval", 1800);
			ColDatSvr[3] = new ColumnDataInt(L"MaxCommandId", 1);
#ifdef WIN32
			ColDatSvr[4] = new ColumnDataWStr(L"BucketPath", L"\\");
#else
			ColDatSvr[4] = new ColumnDataWStr(L"BucketPath", L"/");
#endif
			RecordData* RecSvrInfo = new RecordData(L"ServerInfo", ColDatSvr, 5);
			// Add record
			LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecSvrInfo);
			UnlockTable(L"ServerInfo");
			delete RecSvrInfo;
		}
		// For user table
		{
			ColumnData *ColDatUser[4];
			ColDatUser[0] = new ColumnDataWStr(L"Name", L"admin@a.a");
			ColDatUser[1] = new ColumnDataWStr(L"Password", L"manager");
			ColDatUser[2] = new ColumnDataInt(L"Role", 0);
			ColDatUser[3] = new ColumnDataWStr(L"Url", L"");
			RecordData* RecUser = new RecordData(L"User", ColDatUser, 4);
			// Add record
			LockTable(L"User", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecUser);
			UnlockTable(L"User");
			delete RecUser;
		}
		{
			ColumnData *ColDatUser[4];
			ColDatUser[0] = new ColumnDataWStr(L"Name", L"takeuchi@a.a");
			ColDatUser[1] = new ColumnDataWStr(L"Password", L"takeuchi");
			ColDatUser[2] = new ColumnDataInt(L"Role", 1);
			ColDatUser[3] = new ColumnDataWStr(L"Url", L"");
			RecordData* RecUser = new RecordData(L"User", ColDatUser, 4);
			// Add record
			LockTable(L"User", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecUser);
			UnlockTable(L"User");
			delete RecUser;
		}

	} else {
		LockAllTable(2);
		int Ret = LoadData(Buf);
		UnlockAllTable();

		if (Ret != 0) {
			return -1;
		}
	}

	return 0;
}

bool DataAccess::CheckExistenceOfTargetAgent(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtInfo[1] = {};
	ColDatAgtInfo[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDat = new RecordData(L"AgentInfo", ColDatAgtInfo, 1);
	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatRes = GetRecord(RecDat);
	UnlockTable(L"AgentInfo");
	if (RecDatRes == NULL) {
		delete RecDat;
		return false;
	}
	delete RecDat;
	delete RecDatRes;
	return true;
}

// Return 0:New Agent, 1:Existing Agent
int DataAccess::SetAgentInfo(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int AgtStatus, wchar_t TimeUtc[DA_MAXLEN_OF_TIME], wchar_t TimeLocal[DA_MAXLEN_OF_TIME])
{
	wchar_t UdTimeUtc[DA_MAXLEN_OF_TIME] = L"";
	wchar_t UdTimeLocal[DA_MAXLEN_OF_TIME] = L"";
	StkPlGetWTimeInIso8601(UdTimeUtc, false);
	StkPlGetWTimeInIso8601(UdTimeLocal, true);
	// Record information
	ColumnData *ColDatAgt[10];
	int RetMode = 0;
	if (CheckExistenceOfTargetAgent(AgtName) == false) {
		// Add record
		ColDatAgt[0] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[1] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[2] = new ColumnDataInt(L"StatusCmd", -1);
		ColDatAgt[3] = new ColumnDataInt(L"OpStatus", -1);
		ColDatAgt[4] = new ColumnDataInt(L"OpCmd", -1);
		ColDatAgt[5] = new ColumnDataBin(L"ReqTime", (unsigned char*)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_REQTIME);
		ColDatAgt[6] = new ColumnDataWStr(L"TimeUtc", TimeUtc);
		ColDatAgt[7] = new ColumnDataWStr(L"TimeLocal", TimeLocal);
		ColDatAgt[8] = new ColumnDataWStr(L"UdTimeUtc", UdTimeUtc);
		ColDatAgt[9] = new ColumnDataWStr(L"UdTimeLocal", UdTimeLocal);
		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 10);

		LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
		int Ret = InsertRecord(RecDatAgt);
		UnlockTable(L"AgentInfo");
		delete RecDatAgt;
		RetMode = 0;
	} else {
		// Update record
		ColumnData *ColDatAgtFind[1];
		ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
		RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

		ColDatAgt[0] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[1] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[2] = new ColumnDataWStr(L"TimeUtc", TimeUtc);
		ColDatAgt[3] = new ColumnDataWStr(L"TimeLocal", TimeLocal);
		ColDatAgt[4] = new ColumnDataWStr(L"UdTimeUtc", UdTimeUtc);
		ColDatAgt[5] = new ColumnDataWStr(L"UdTimeLocal", UdTimeLocal);
		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 6);

		LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
		int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
		UnlockTable(L"AgentInfo");
		delete RecDatAgt;
		delete RecDatAgtFind;
		RetMode = 1;
	}
	return RetMode;
}

void DataAccess::SetAgentInfoForStatusCmd(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int StatusCmd)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[2];
	ColDatAgt[0] = new ColumnDataInt(L"StatusCmd", StatusCmd);
	ColDatAgt[1] = new ColumnDataInt(L"Status", -982);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 2);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
}

void DataAccess::SetAgentInfoForStatus(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int Status)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataInt(L"Status", Status);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
}

void DataAccess::SetAgentInfoForOpCmd(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int OpCmd)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[2];
	ColDatAgt[0] = new ColumnDataInt(L"OpCmd", OpCmd);
	ColDatAgt[1] = new ColumnDataInt(L"OpStatus", -983);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 2);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;

	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
	GetCommandNameById(OpCmd, CmdName);
}

void DataAccess::SetAgentInfoForOpStatus(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int OpStatus)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataInt(L"OpStatus", OpStatus);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
}

void DataAccess::SetAgentInfoForReqTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	long long ReqTime = StkPlGetTime();
	unsigned char ReqTimeBin[DA_MAXLEN_OF_REQTIME] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	StkPlMemCpy((void*)ReqTimeBin, (void*)&ReqTime, 8);
	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataBin(L"ReqTime", ReqTimeBin, DA_MAXLEN_OF_REQTIME);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
}

int DataAccess::GetAgentInfo(wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
							int AgtStatus[DA_MAXNUM_OF_AGTRECORDS],
							int AgtStatusCmd[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpStatus[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpCmd[DA_MAXNUM_OF_AGTRECORDS],
							wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
							wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
							wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
							wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME])
{
	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"AgentInfo");
	UnlockTable(L"AgentInfo");

	int NumOfRec = 0;
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataWStr* ColDatName = (ColumnDataWStr*)CurrRecDat->GetColumn(0);
		ColumnDataInt* ColDatStatus = (ColumnDataInt*)CurrRecDat->GetColumn(1);
		ColumnDataInt* ColDatStatusCmd = (ColumnDataInt*)CurrRecDat->GetColumn(2);
		ColumnDataInt* ColDatOpStatus = (ColumnDataInt*)CurrRecDat->GetColumn(3);
		ColumnDataInt* ColDatOpCmd = (ColumnDataInt*)CurrRecDat->GetColumn(4);
		ColumnDataBin* ColDatReqTime = (ColumnDataBin*)CurrRecDat->GetColumn(5);
		ColumnDataWStr* ColDatTimeUtc = (ColumnDataWStr*)CurrRecDat->GetColumn(6);
		ColumnDataWStr* ColDatTimeLocal = (ColumnDataWStr*)CurrRecDat->GetColumn(7);
		ColumnDataWStr* ColDatUdTimeUtc = (ColumnDataWStr*)CurrRecDat->GetColumn(8);
		ColumnDataWStr* ColDatUdTimeLocal = (ColumnDataWStr*)CurrRecDat->GetColumn(9);
		if (ColDatTimeUtc != NULL && ColDatTimeUtc->GetValue() != NULL) {
			StkPlSwPrintf(TimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, ColDatTimeUtc->GetValue());
		} else {
			StkPlSwPrintf(TimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatStatus != NULL) {
			AgtStatus[NumOfRec] = ColDatStatus->GetValue();
		} else {
			AgtStatus[NumOfRec] = -1;
		}
		if (ColDatStatusCmd != NULL) {
			AgtStatusCmd[NumOfRec] = ColDatStatusCmd->GetValue();
		} else {
			AgtStatusCmd[NumOfRec] = -1;
		}
		if (ColDatOpStatus != NULL) {
			OpStatus[NumOfRec] = ColDatOpStatus->GetValue();
		} else {
			OpStatus[NumOfRec] = -1;
		}
		if (ColDatOpCmd != NULL) {
			OpCmd[NumOfRec] = ColDatOpCmd->GetValue();
		} else {
			OpCmd[NumOfRec] = -1;
		}
		if (ColDatTimeLocal != NULL && ColDatTimeLocal->GetValue() != NULL) {
			StkPlSwPrintf(TimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, ColDatTimeLocal->GetValue());
		} else {
			StkPlSwPrintf(TimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatUdTimeUtc != NULL && ColDatUdTimeUtc->GetValue() != NULL) {
			StkPlSwPrintf(UdTimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, ColDatUdTimeUtc->GetValue());
		} else {
			StkPlSwPrintf(UdTimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatUdTimeLocal != NULL && ColDatUdTimeLocal->GetValue() != NULL) {
			StkPlSwPrintf(UdTimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, ColDatUdTimeLocal->GetValue());
		} else {
			StkPlSwPrintf(UdTimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatName != NULL && ColDatName->GetValue() != NULL) {
			StkPlSwPrintf(AgtName[NumOfRec], DA_MAXLEN_OF_LOGMSG, ColDatName->GetValue());
		} else {
			StkPlSwPrintf(AgtName[NumOfRec], DA_MAXLEN_OF_LOGMSG, L"");
		}
		NumOfRec++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfRec;
}

int DataAccess::GetAgentInfoForOpCmd(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatAgt = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");

	int OpCmd = -1;
	if (RecDatAgt != NULL) {
		ColumnDataInt* ColDatOpCmd = (ColumnDataInt*)RecDatAgt->GetColumn(4);
		if (ColDatOpCmd != NULL) {
			OpCmd = ColDatOpCmd->GetValue();
		}
	}

	delete RecDatAgtFind;
	delete RecDatAgt;

	return OpCmd;
}

int DataAccess::GetAgentInfoForOpStatus(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatAgt = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");

	int OpStatus = -1;
	if (RecDatAgt != NULL) {
		ColumnDataInt* ColDatOpStatus = (ColumnDataInt*)RecDatAgt->GetColumn(3);
		if (ColDatOpStatus != NULL) {
			OpStatus = ColDatOpStatus->GetValue();
		}
	}

	delete RecDatAgtFind;
	delete RecDatAgt;

	return OpStatus;
}

long long DataAccess::GetAgentInfoForReqTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatAgt = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");

	long long ReqTime = -1;
	if (RecDatAgt != NULL) {
		ColumnDataBin* ColDatReqTime = (ColumnDataBin*)RecDatAgt->GetColumn(5);
		if (ColDatReqTime != NULL) {
			long long *ReqTimePtr = (long long*)ColDatReqTime->GetValue();
			ReqTime = *ReqTimePtr;
		}
	}

	delete RecDatAgtFind;
	delete RecDatAgt;

	return ReqTime;
}

int  DataAccess::GetServerInfo(int* PInterval, int* SaInterval, wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH])
{
	LockTable(L"ServerInfo", LOCK_SHARE);
	RecordData* RecDatSvr = GetRecord(L"ServerInfo");
	UnlockTable(L"ServerInfo");
	if (RecDatSvr == NULL) {
		return -1;
	}
	ColumnDataInt* ColDatPInterval = (ColumnDataInt*)RecDatSvr->GetColumn(1);
	ColumnDataInt* ColDatSaInterval = (ColumnDataInt*)RecDatSvr->GetColumn(2);
	ColumnDataWStr* ColDatBucketPath = (ColumnDataWStr*)RecDatSvr->GetColumn(4);
	if (ColDatPInterval == NULL || ColDatSaInterval == NULL || ColDatBucketPath == NULL) {
		delete RecDatSvr;
		return -1;
	}
	*PInterval = ColDatPInterval->GetValue();
	*SaInterval = ColDatSaInterval->GetValue();
	StkPlWcsCpy(BucketPath, DA_MAXLEN_OF_BUCKETPATH, ColDatBucketPath->GetValue());
	delete RecDatSvr;
	return 0;
}

int DataAccess::SetServerInfo(int PInterval, int SaInterval, wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH])
{
	ColumnData *ColDatSvrFind[1];
	ColDatSvrFind[0] = new ColumnDataInt(L"Id", 0);
	RecordData* RecDatSvrFind = new RecordData(L"ServerInfo", ColDatSvrFind, 1);

	ColumnData *ColDatSvr[3];
	ColDatSvr[0] = new ColumnDataInt(L"PInterval", PInterval);
	ColDatSvr[1] = new ColumnDataInt(L"SaInterval", SaInterval);
	ColDatSvr[2] = new ColumnDataWStr(L"BucketPath", BucketPath);
	RecordData* RecDatSvr = new RecordData(L"ServerInfo", ColDatSvr, 3);

	LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSvrFind, RecDatSvr);
	UnlockTable(L"ServerInfo");
	delete RecDatSvr;
	delete RecDatSvrFind;

	return 0;
}

void DataAccess::GetFullPathFromFileName(wchar_t FullPath[DA_MAXLEN_OF_SERVERFILENAME], const wchar_t FileName[DA_MAXLEN_OF_SERVERFILENAME])
{
	LockTable(L"ServerInfo", LOCK_SHARE);
	RecordData* RecDatSvrInfo = GetRecord(L"ServerInfo");
	UnlockTable(L"ServerInfo");
	int MaxCommandId = 0;
	if (RecDatSvrInfo != NULL) {
		ColumnDataWStr* ColDat = (ColumnDataWStr*)RecDatSvrInfo->GetColumn(4);
		if (ColDat != NULL) {
			StkPlWcsCpy(FullPath, DA_MAXLEN_OF_SERVERFILENAME, ColDat->GetValue());
			int FullPathLen = StkPlWcsLen(FullPath);
			if (FullPathLen >= 1) {
#ifdef WIN32
				if (FullPath[FullPathLen - 1] != L'\\') {
					StkPlWcsCat(FullPath, DA_MAXLEN_OF_SERVERFILENAME, L"\\");
				}
#else
				if (FullPath[FullPathLen - 1] != L'/') {
					StkPlWcsCat(FullPath, DA_MAXLEN_OF_SERVERFILENAME, L"/");
				}
#endif
			}
			StkPlWcsCat(FullPath, DA_MAXLEN_OF_SERVERFILENAME, FileName);
		}
	}
	delete RecDatSvrInfo;
}

int DataAccess::GetCommand(int Id[DA_MAXNUM_OF_CMDRECORDS], wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME], int Type[DA_MAXNUM_OF_CMDRECORDS], char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT], wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME], wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME])
{
	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdRes = GetRecord(L"Command");
	UnlockTable(L"Command");

	int NumOfRec = 0;
	RecordData* CurDat = RecDatCmdRes;
	while (CurDat) {
		ColumnDataInt* ColDatCmdResId = (ColumnDataInt*)CurDat->GetColumn(0);
		ColumnDataWStr* ColDatCmdResName = (ColumnDataWStr*)CurDat->GetColumn(1);
		ColumnDataInt* ColDatCmdResType = (ColumnDataInt*)CurDat->GetColumn(2);
		ColumnDataBin* ColDatCmdResScript = (ColumnDataBin*)CurDat->GetColumn(3);
		ColumnDataWStr* ColDatCmdResServerFileName = (ColumnDataWStr*)CurDat->GetColumn(4);
		ColumnDataWStr* ColDatCmdResAgentFileName = (ColumnDataWStr*)CurDat->GetColumn(5);
		if (ColDatCmdResId == NULL || ColDatCmdResName == NULL || ColDatCmdResType == NULL || ColDatCmdResScript == NULL || ColDatCmdResServerFileName == NULL || ColDatCmdResAgentFileName == NULL) {
			break;
		}
		Id[NumOfRec] = ColDatCmdResId->GetValue();
		StkPlWcsCpy(Name[NumOfRec], DA_MAXLEN_OF_CMDNAME, ColDatCmdResName->GetValue());
		Type[NumOfRec] = ColDatCmdResType->GetValue();
		StkPlMemCpy(Script[NumOfRec], ColDatCmdResScript->GetValue(), DA_MAXLEN_OF_CMDSCRIPT);
		StkPlWcsCpy(ServerFileName[NumOfRec], DA_MAXLEN_OF_SERVERFILENAME, ColDatCmdResServerFileName->GetValue());
		StkPlWcsCpy(AgentFileName[NumOfRec], DA_MAXLEN_OF_AGENTFILENAME, ColDatCmdResAgentFileName->GetValue());

		NumOfRec++;
		CurDat = CurDat->GetNextRecord();
	}

	delete RecDatCmdRes;
	return NumOfRec;
}

int DataAccess::GetCommandNameById(int Id, wchar_t Name[DA_MAXLEN_OF_CMDNAME])
{
	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);

	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	UnlockTable(L"Command");
	delete RecDatCmdFind;
	if (RecDatCmdFindRes != NULL) {
		ColumnDataWStr* NameObj = (ColumnDataWStr*)RecDatCmdFindRes->GetColumn(L"Name");
		StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, NameObj->GetValue());
		delete RecDatCmdFindRes;
	} else {
		return -1;
	}

	return 0;
}

bool DataAccess::CheckCommandExistenceByName(wchar_t Name[DA_MAXLEN_OF_CMDNAME])
{
	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataWStr(L"Name", Name);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);

	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	UnlockTable(L"Command");
	delete RecDatCmdFind;
	if (RecDatCmdFindRes != NULL) {
		delete RecDatCmdFindRes;
		return true;
	}
	delete RecDatCmdFindRes;
	return false;
}

// Return: 0:Added, 1:Modified
int DataAccess::SetCommand(int Id, wchar_t Name[DA_MAXLEN_OF_CMDNAME], int Type, char Script[DA_MAXLEN_OF_CMDSCRIPT], wchar_t ServerFileName[DA_MAXLEN_OF_SERVERFILENAME], wchar_t AgentFileName[DA_MAXLEN_OF_AGENTFILENAME])
{
	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);
	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	UnlockTable(L"Command");

	ColumnData *ColDatCmd[6];
	ColDatCmd[0] = new ColumnDataInt(L"Id", Id);
	ColDatCmd[1] = new ColumnDataWStr(L"Name", Name);
	ColDatCmd[2] = new ColumnDataInt(L"Type", Type);
	ColDatCmd[3] = new ColumnDataBin(L"Script", (unsigned char*)Script, DA_MAXLEN_OF_CMDSCRIPT);
	ColDatCmd[4] = new ColumnDataWStr(L"ServerFileName", ServerFileName);
	ColDatCmd[5] = new ColumnDataWStr(L"AgentFileName", AgentFileName);
	RecordData* RecDatCmd = new RecordData(L"Command", ColDatCmd, 6);
	LockTable(L"Command", LOCK_EXCLUSIVE);
	int Ret = 0;
	if (RecDatCmdFindRes == NULL) {
		InsertRecord(RecDatCmd);
		Ret = 0;
	} else {
		UpdateRecord(RecDatCmdFind, RecDatCmd);
		Ret = 1;
	}
	UnlockTable(L"Command");
	delete RecDatCmd;

	delete RecDatCmdFind;
	delete RecDatCmdFindRes;

	return Ret;
}

int DataAccess::DeleteCommand(int Id)
{
	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);

	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	UnlockTable(L"Command");
	if (RecDatCmdFindRes != NULL) {
		delete RecDatCmdFindRes;

		LockTable(L"Command", LOCK_EXCLUSIVE);
		DeleteRecord(RecDatCmdFind);
		UnlockTable(L"Command");
		delete RecDatCmdFind;
	} else {
		delete RecDatCmdFind;
		return -1;
	}

	return 0;
}

int DataAccess::GetMaxCommandId()
{
	LockTable(L"ServerInfo", LOCK_SHARE);
	RecordData* RecDatCommand = GetRecord(L"ServerInfo");
	UnlockTable(L"ServerInfo");
	int MaxCommandId = 0;
	if (RecDatCommand != NULL) {
		ColumnDataInt* ColDat = (ColumnDataInt*)RecDatCommand->GetColumn(3);
		if (ColDat != NULL) {
			MaxCommandId = ColDat->GetValue();
		}
	}
	delete RecDatCommand;
	return MaxCommandId;
}

int DataAccess::SetMaxCommandId(int Id)
{
	ColumnData *ColDatSvrFind[1];
	ColDatSvrFind[0] = new ColumnDataInt(L"Id", 0);
	RecordData* RecDatSvrFind = new RecordData(L"ServerInfo", ColDatSvrFind, 1);

	ColumnData *ColDatSvr[1];
	ColDatSvr[0] = new ColumnDataInt(L"MaxCommandId", Id);
	RecordData* RecDatSvr = new RecordData(L"ServerInfo", ColDatSvr, 1);

	LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSvrFind, RecDatSvr);
	UnlockTable(L"ServerInfo");
	delete RecDatSvr;
	delete RecDatSvrFind;
	return Id;
}

// Add log message
// LogMsg [in] : Message which you want to insert
// Return : always zero returned.
int DataAccess::AddLogMsg(const wchar_t LogMsg[DA_MAXLEN_OF_LOGMSG], const wchar_t LogMsgJa[DA_MAXLEN_OF_LOGMSG])
{
	static int MaxLogId = 0;
	if (MaxLogId == 0) {
		MaxLogId = GetMaxLogId() + 1;
	}
	static int DelCnt = 0;
	if (DelCnt == 0) {
		DeleteOldLogs();
	}
	DelCnt++;
	if (DelCnt == 10) {
		DelCnt = 0;
	}

	wchar_t TimeLocalBuf[64] = L"";
	wchar_t TimeUtcBuf[64] = L"";
	StkPlGetWTimeInIso8601(TimeLocalBuf, true);
	StkPlGetWTimeInIso8601(TimeUtcBuf, false);
	// New record information
	ColumnData *ColDatLog[5];
	ColDatLog[0] = new ColumnDataInt(L"Id", MaxLogId);
	ColDatLog[1] = new ColumnDataWStr(L"TimeUtc", TimeUtcBuf);
	ColDatLog[2] = new ColumnDataWStr(L"TimeLocal", TimeLocalBuf);
	ColDatLog[3] = new ColumnDataWStr(L"Message", LogMsg);
	ColDatLog[4] = new ColumnDataWStr(L"MessageJa", LogMsgJa);
	RecordData* RecDatLog = new RecordData(L"Log", ColDatLog, 5);
	// Add record
	LockTable(L"Log", LOCK_EXCLUSIVE);
	int Ret = InsertRecord(RecDatLog);
	UnlockTable(L"Log");
	delete RecDatLog;
	MaxLogId++;
	return 0;
}

// Get maximum log id.
// This method checks all of registered log ids and returns the maximum id
// Return : Maximum log id
int DataAccess::GetMaxLogId()
{
	LockTable(L"Log", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	RecordData* CurrRecDat = RecDatLog;
	int MaxLogId = 0;
	while (CurrRecDat != NULL) {
		ColumnDataInt* ColDat = (ColumnDataInt*)CurrRecDat->GetColumn(0);
		int CurrId = ColDat->GetValue();
		if (CurrId > MaxLogId) {
			MaxLogId = CurrId;
		}
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return MaxLogId;
}

// Get number of logs.
// This method gets number of logs
// Return : Number of logs
int DataAccess::GetNumOfLogs()
{
	LockTable(L"Log", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	RecordData* CurrRecDat = RecDatLog;
	int NumOfLogs = 0;
	while (CurrRecDat != NULL) {
		NumOfLogs++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfLogs;
}

// Get log information
// LogMsgTimeUtc [out] : Acquired UTC time
// LogMsgTimeLocal [out] : Acquired local time
// LogMsg [out] : Acquired log message
// Return : Number of acquired log messages
int DataAccess::GetLogs(
	wchar_t LogMsgTimeUtc[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME],
	wchar_t LogMsgTimeLocal[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME],
	wchar_t LogMsg[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG],
	wchar_t LogMsgJa[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG])
{
	LockTable(L"Log", LOCK_EXCLUSIVE);
	AzSortRecord(L"Log", L"Id");
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	int NumOfRec = 0;
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataWStr* ColDatTimeUtc = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
		ColumnDataWStr* ColDatTimeLocal = (ColumnDataWStr*)CurrRecDat->GetColumn(2);
		ColumnDataWStr* ColDatMsg = (ColumnDataWStr*)CurrRecDat->GetColumn(3);
		ColumnDataWStr* ColDatMsgJa = (ColumnDataWStr*)CurrRecDat->GetColumn(4);
		if (ColDatTimeUtc != NULL && ColDatTimeUtc->GetValue() != NULL) {
			StkPlSwPrintf(LogMsgTimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, ColDatTimeUtc->GetValue());
		} else {
			StkPlSwPrintf(LogMsgTimeUtc[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatTimeLocal != NULL && ColDatTimeLocal->GetValue() != NULL) {
			StkPlSwPrintf(LogMsgTimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, ColDatTimeLocal->GetValue());
		} else {
			StkPlSwPrintf(LogMsgTimeLocal[NumOfRec], DA_MAXLEN_OF_TIME, L"");
		}
		if (ColDatMsg != NULL && ColDatMsg->GetValue() != NULL) {
			StkPlSwPrintf(LogMsg[NumOfRec], DA_MAXLEN_OF_LOGMSG, ColDatMsg->GetValue());
		} else {
			StkPlSwPrintf(LogMsg[NumOfRec], DA_MAXLEN_OF_LOGMSG, L"");
		}
		if (ColDatMsgJa != NULL && ColDatMsgJa->GetValue() != NULL) {
			StkPlSwPrintf(LogMsgJa[NumOfRec], DA_MAXLEN_OF_LOGMSG, ColDatMsgJa->GetValue());
		} else {
			StkPlSwPrintf(LogMsgJa[NumOfRec], DA_MAXLEN_OF_LOGMSG, L"");
		}
		NumOfRec++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfRec;
}

// This function deletes old logs.
// Return : Always zero returned
int DataAccess::DeleteOldLogs()
{
	int NumOfLogs = GetNumOfLogs();
	if (NumOfLogs >= DA_MAXNUM_OF_LOGRECORDS - 10) {
		LockTable(L"Log", LOCK_EXCLUSIVE);
		AzSortRecord(L"Log", L"Id");
		RecordData* RecDatLog = GetRecord(L"Log");
		UnlockTable(L"Log");
		int ExceededNumOfLogs = NumOfLogs - (DA_MAXNUM_OF_LOGRECORDS - 10);
		RecordData* CurrRecDat = RecDatLog;
		for (int Loop = 0; Loop < ExceededNumOfLogs; Loop++) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)CurrRecDat->GetColumn(0);
			int ValueId = ColDatId->GetValue();

			ColumnData* DelColDat[1];
			DelColDat[0] = new ColumnDataInt(L"Id", ValueId);
			RecordData* DelRecDat = new RecordData(L"Log", DelColDat, 1);
			LockTable(L"Log", LOCK_EXCLUSIVE);
			DeleteRecord(DelRecDat);
			UnlockTable(L"Log");
			delete DelRecDat;

			CurrRecDat = CurrRecDat->GetNextRecord();
		}
		delete RecDatLog;
	}

	return 0;
}

bool DataAccess::GetTargetUser(wchar_t* Name, wchar_t* Password, int* Role, wchar_t* Url)
{
	ColumnData* ColDat[1];
	ColDat[0] = new ColumnDataWStr(L"Name", Name);
	RecordData* SearchUser = new RecordData(L"User", ColDat, 1);
	LockTable(L"User", LOCK_SHARE);
	RecordData* RecDatUser = GetRecord(SearchUser);
	UnlockTable(L"User");
	delete SearchUser;
	if (RecDatUser != NULL) {
		ColumnDataWStr* ColDatPw = (ColumnDataWStr*)RecDatUser->GetColumn(1);
		ColumnDataInt* ColDatRole = (ColumnDataInt*)RecDatUser->GetColumn(2);
		ColumnDataWStr* ColDatUrl = (ColumnDataWStr*)RecDatUser->GetColumn(3);
		if (ColDatPw != NULL && ColDatRole != NULL && ColDatUrl != NULL) {
			StkPlWcsCpy(Password, DA_MAXLEN_OF_PASSWORD, ColDatPw->GetValue());
			*Role = ColDatRole->GetValue();
			StkPlWcsCpy(Url, DA_MAXLEN_OF_PASSWORD, ColDatUrl->GetValue());
		}
	} else {
		return false;
	}
	delete RecDatUser;
	return true;
}
