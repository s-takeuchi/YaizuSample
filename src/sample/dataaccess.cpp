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
			TableDef TabDefLog(L"Log", DA_MAXNUM_OF_LOGRECORDS);
			TabDefLog.AddColumnDef(&ColDefLogId);
			TabDefLog.AddColumnDef(&ColDefLogTimeUtc);
			TabDefLog.AddColumnDef(&ColDefLogTimeLocal);
			TabDefLog.AddColumnDef(&ColDefLogMsg);
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
			TableDef TabDefSvrInfo(L"ServerInfo", 1);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrId);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrPi);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrSai);
			TabDefSvrInfo.AddColumnDef(&ColDefSvrMaxComId);
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
		UnlockAllTable();

		{
			ColumnData *ColDatSvr[4];
			ColDatSvr[0] = new ColumnDataInt(L"Id", 0);
			ColDatSvr[1] = new ColumnDataInt(L"PInterval", 300);
			ColDatSvr[2] = new ColumnDataInt(L"SaInterval", 1800);
			ColDatSvr[3] = new ColumnDataInt(L"MaxCommandId", 1);
			RecordData* RecSvrInfo = new RecordData(L"ServerInfo", ColDatSvr, 4);
			// Add record
			LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecSvrInfo);
			UnlockTable(L"ServerInfo");
			delete RecSvrInfo;
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

void DataAccess::SetAgentInfo(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int AgtStatus, wchar_t TimeUtc[DA_MAXLEN_OF_TIME], wchar_t TimeLocal[DA_MAXLEN_OF_TIME])
{
	wchar_t UdTimeUtc[DA_MAXLEN_OF_TIME] = L"";
	wchar_t UdTimeLocal[DA_MAXLEN_OF_TIME] = L"";
	StkPlGetWTimeInIso8601(UdTimeUtc, false);
	StkPlGetWTimeInIso8601(UdTimeLocal, true);
	// Record information
	ColumnData *ColDatAgt[9];
	if (CheckExistenceOfTargetAgent(AgtName) == false) {
		// Add record
		ColDatAgt[0] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[1] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[2] = new ColumnDataInt(L"StatusCmd", -1);
		ColDatAgt[3] = new ColumnDataInt(L"OpStatus", -1);
		ColDatAgt[4] = new ColumnDataInt(L"OpCmd", -1);
		ColDatAgt[5] = new ColumnDataWStr(L"TimeUtc", TimeUtc);
		ColDatAgt[6] = new ColumnDataWStr(L"TimeLocal", TimeLocal);
		ColDatAgt[7] = new ColumnDataWStr(L"UdTimeUtc", UdTimeUtc);
		ColDatAgt[8] = new ColumnDataWStr(L"UdTimeLocal", UdTimeLocal);
		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 9);

		LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
		int Ret = InsertRecord(RecDatAgt);
		UnlockTable(L"AgentInfo");
		delete RecDatAgt;

		wchar_t LogMsg[512] = L"";
		StkPlSwPrintf(LogMsg, 256, L"New agent information has been notified. [%ls]", AgtName);
		AddLogMsg(LogMsg);
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
	}
}

void DataAccess::SetAgentInfoForStatusCmd(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int StatusCmd)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataInt(L"StatusCmd", StatusCmd);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;

	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
	GetCommandNameById(StatusCmd, CmdName);

	wchar_t LogMsg[512] = L"";
	StkPlSwPrintf(LogMsg, 256, L"Command for status acquisition has been changed. [%ls, %ls]", AgtName, CmdName);
	AddLogMsg(LogMsg);
}

void DataAccess::SetAgentInfoForOpCmd(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int OpCmd)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataInt(L"OpCmd", OpCmd);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;

	wchar_t CmdName[DA_MAXLEN_OF_CMDNAME] = L"";
	GetCommandNameById(OpCmd, CmdName);

	wchar_t LogMsg[512] = L"";
	if (OpCmd == -1) {
		StkPlSwPrintf(LogMsg, 256, L"Command for operation has ended. [%ls]", AgtName);
	} else {
		StkPlSwPrintf(LogMsg, 256, L"Command for operation has started. [%ls, %ls]", AgtName, CmdName);
	}
	AddLogMsg(LogMsg);
}

int DataAccess::GetAgentInfo(wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
							int AgtStatus[DA_MAXNUM_OF_AGTRECORDS],
							int AgtStatusCmd[DA_MAXNUM_OF_AGTRECORDS],
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
		ColumnDataWStr* ColDatTimeUtc = (ColumnDataWStr*)CurrRecDat->GetColumn(5);
		ColumnDataWStr* ColDatTimeLocal = (ColumnDataWStr*)CurrRecDat->GetColumn(6);
		ColumnDataWStr* ColDatUdTimeUtc = (ColumnDataWStr*)CurrRecDat->GetColumn(7);
		ColumnDataWStr* ColDatUdTimeLocal = (ColumnDataWStr*)CurrRecDat->GetColumn(8);
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
			//
		} else {
			//
		}
		if (ColDatOpCmd != NULL) {
			//
		} else {
			//
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

int  DataAccess::GetServerInfo(int* PInterval, int* SaInterval)
{
	LockTable(L"ServerInfo", LOCK_SHARE);
	RecordData* RecDatSvr = GetRecord(L"ServerInfo");
	UnlockTable(L"ServerInfo");
	if (RecDatSvr == NULL) {
		return -1;
	}
	ColumnDataInt* ColDatPInterval = (ColumnDataInt*)RecDatSvr->GetColumn(1);
	ColumnDataInt* ColDatSaInterval = (ColumnDataInt*)RecDatSvr->GetColumn(2);
	if (ColDatPInterval == NULL || ColDatSaInterval == NULL) {
		delete RecDatSvr;
		return -1;
	}
	*PInterval = ColDatPInterval->GetValue();
	*SaInterval = ColDatSaInterval->GetValue();
	delete RecDatSvr;
	return 0;
}

int DataAccess::SetServerInfo(int PInterval, int SaInterval)
{
	ColumnData *ColDatSvrFind[1];
	ColDatSvrFind[0] = new ColumnDataInt(L"Id", 0);
	RecordData* RecDatSvrFind = new RecordData(L"ServerInfo", ColDatSvrFind, 1);

	ColumnData *ColDatSvr[2];
	ColDatSvr[0] = new ColumnDataInt(L"PInterval", PInterval);
	ColDatSvr[1] = new ColumnDataInt(L"SaInterval", SaInterval);
	RecordData* RecDatSvr = new RecordData(L"ServerInfo", ColDatSvr, 2);

	LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSvrFind, RecDatSvr);
	UnlockTable(L"ServerInfo");
	delete RecDatSvr;
	delete RecDatSvrFind;

	wchar_t LogMsg[256] = L"";
	StkPlSwPrintf(LogMsg, 256, L"Server information has been changed. [Polling Interval=%d sec, Status Acquisition Interval=%d sec]", PInterval, SaInterval);
	AddLogMsg(LogMsg);

	return 0;
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
	if (RecDatCmdFindRes != NULL) {
		ColumnDataWStr* NameObj = (ColumnDataWStr*)RecDatCmdFindRes->GetColumn(L"Name");
		StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, NameObj->GetValue());
		delete RecDatCmdFindRes;
	}
	delete RecDatCmdFind;

	return 0;
}

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
	wchar_t LogMsg[256] = L"";
	if (RecDatCmdFindRes == NULL) {
		InsertRecord(RecDatCmd);
		StkPlSwPrintf(LogMsg, 256, L"A command has been added. [%ls]", Name);
	} else {
		UpdateRecord(RecDatCmdFind, RecDatCmd);
		StkPlSwPrintf(LogMsg, 256, L"A command has been modified. [%ls]", Name);
	}
	UnlockTable(L"Command");
	delete RecDatCmd;

	delete RecDatCmdFind;
	delete RecDatCmdFindRes;

	AddLogMsg(LogMsg);

	return 0;
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
		ColumnDataWStr* Name = (ColumnDataWStr*)RecDatCmdFindRes->GetColumn(L"Name");
		wchar_t LogMsg[256] = L"";
		StkPlSwPrintf(LogMsg, 256, L"A command has been deleted. [%ls]", Name->GetValue());
		AddLogMsg(LogMsg);
		delete RecDatCmdFindRes;

		LockTable(L"Command", LOCK_EXCLUSIVE);
		DeleteRecord(RecDatCmdFind);
		UnlockTable(L"Command");
	}
	delete RecDatCmdFind;

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
int DataAccess::AddLogMsg(const wchar_t LogMsg[DA_MAXLEN_OF_LOGMSG])
{
	static int MaxLogId = 0;
	if (MaxLogId == 0) {
		MaxLogId = GetMaxLogId() + 1;
		DeleteOldLogs();
	}
	static int DelFlag = 0;
	if (DelFlag == 10) {
		DeleteOldLogs();
		DelFlag = 0;
	} else {
		DelFlag++;
	}

	wchar_t TimeLocalBuf[64] = L"";
	wchar_t TimeUtcBuf[64] = L"";
	StkPlGetWTimeInIso8601(TimeLocalBuf, true);
	StkPlGetWTimeInIso8601(TimeUtcBuf, false);
	// New record information
	ColumnData *ColDatLog[4];
	ColDatLog[0] = new ColumnDataInt(L"Id", MaxLogId);
	ColDatLog[1] = new ColumnDataWStr(L"TimeUtc", TimeUtcBuf);
	ColDatLog[2] = new ColumnDataWStr(L"TimeLocal", TimeLocalBuf);
	ColDatLog[3] = new ColumnDataWStr(L"Message", LogMsg);
	RecordData* RecDatLog = new RecordData(L"Log", ColDatLog, 4);
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
	wchar_t LogMsg[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG])
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
	if (NumOfLogs >= DA_MAXNUM_OF_LOGRECORDS - 100) {
		LockTable(L"Log", LOCK_EXCLUSIVE);
		AzSortRecord(L"Log", L"Id");
		RecordData* RecDatLog = GetRecord(L"Log");
		UnlockTable(L"Log");
		int ExceededNumOfLogs = NumOfLogs - (DA_MAXNUM_OF_LOGRECORDS - 101);
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
