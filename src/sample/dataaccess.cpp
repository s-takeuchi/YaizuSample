#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkdata/stkdata.h"
#include "../../../YaizuComLib/src/stkdata/stkdataapi.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "serval.h"
#include "dataaccess.h"

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
// DataFileName [in] : data file name which you want to preserve. Do NOT specify path to file.
// Return : [0:Necessary tables are created, 1:Existing data file is loaded, -1:Failed, -2:Data file invalid]
int DataAccess::CreateTables(const wchar_t* DataFileName)
{
	// Make full path name and call AutoSave
	wchar_t Buf[FILENAME_MAX];
	StkPlGetFullPathFromFileName(DataFileName, Buf);
	size_t WorkDatLength = StkPlGetFileSize(Buf);
	if (WorkDatLength == (size_t)-1) {
#ifdef WIN32
		return -1;
#endif
#ifndef WIN32
		StkPlSwPrintf(Buf, FILENAME_MAX, L"/etc/%ls", DataFileName);
#endif
	}

	if (StkPlGetFileSize(Buf) == 0) {
		LockAllTable(2);
		// Agent info table
		{
			ColumnDefInt  ColDefAgtId(L"Id");
			ColumnDefWStr ColDefAgtName(L"Name", DA_MAXLEN_OF_AGTNAME);
			ColumnDefInt  ColDefAgtStatus(L"Status");
			ColumnDefInt  ColDefAgtStatusCmd(L"StatusCmd");
			ColumnDefInt  ColDefAgtOpStatus(L"OpStatus");
			ColumnDefInt  ColDefAgtOpCmd(L"OpCmd");
			ColumnDefBin  ColDefAgtReqTime(L"ReqTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtAcqTime(L"AcqTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtUpdTime(L"UpdTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtIniTime(L"IniTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtOpeTime(L"OpeTime", DA_MAXLEN_OF_UNIXTIME);
			TableDef TabDefAgtInfo(L"AgentInfo", DA_MAXNUM_OF_AGTRECORDS);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtId);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtName);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatusCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtReqTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtAcqTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtUpdTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtIniTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpeTime);
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
			ColumnDefWStr ColDefComServerFileName0(L"ServerFileName0", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComServerFileName1(L"ServerFileName1", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComServerFileName2(L"ServerFileName2", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComServerFileName3(L"ServerFileName3", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComServerFileName4(L"ServerFileName4", DA_MAXLEN_OF_SERVERFILENAME);
			ColumnDefWStr ColDefComAgentFileName0(L"AgentFileName0", DA_MAXLEN_OF_AGENTFILENAME);
			ColumnDefWStr ColDefComAgentFileName1(L"AgentFileName1", DA_MAXLEN_OF_AGENTFILENAME);
			ColumnDefWStr ColDefComAgentFileName2(L"AgentFileName2", DA_MAXLEN_OF_AGENTFILENAME);
			ColumnDefWStr ColDefComAgentFileName3(L"AgentFileName3", DA_MAXLEN_OF_AGENTFILENAME);
			ColumnDefWStr ColDefComAgentFileName4(L"AgentFileName4", DA_MAXLEN_OF_AGENTFILENAME);
			ColumnDefInt ColDefComTimeout(L"Timeout");
			TableDef TabDefCommand(L"Command", DA_MAXNUM_OF_CMDRECORDS);
			TabDefCommand.AddColumnDef(&ColDefComId);
			TabDefCommand.AddColumnDef(&ColDefComName);
			TabDefCommand.AddColumnDef(&ColDefComType);
			TabDefCommand.AddColumnDef(&ColDefComScript);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName0);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName1);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName2);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName3);
			TabDefCommand.AddColumnDef(&ColDefComServerFileName4);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName0);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName1);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName2);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName3);
			TabDefCommand.AddColumnDef(&ColDefComAgentFileName4);
			TabDefCommand.AddColumnDef(&ColDefComTimeout);
			if (CreateTable(&TabDefCommand) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Result table
		{
			ColumnDefInt ColDefId(L"Id");
			ColumnDefBin ColDefUpdTime(L"UpdTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefInt ColDefType(L"Type");
			ColumnDefWStr ColDefComName(L"CmdName", DA_MAXLEN_OF_CMDNAME);
			ColumnDefWStr ColDefAgtName(L"AgtName", DA_MAXLEN_OF_AGTNAME);
			ColumnDefInt ColDefStatus(L"Status");
			ColumnDefInt ColDefExitCode(L"ExitCode");
			TableDef TabDefResult(L"Result", DA_MAXNUM_OF_RESULT);
			TabDefResult.AddColumnDef(&ColDefId);
			TabDefResult.AddColumnDef(&ColDefUpdTime);
			TabDefResult.AddColumnDef(&ColDefType);
			TabDefResult.AddColumnDef(&ColDefComName);
			TabDefResult.AddColumnDef(&ColDefAgtName);
			TabDefResult.AddColumnDef(&ColDefStatus);
			TabDefResult.AddColumnDef(&ColDefExitCode);
			if (CreateTable(&TabDefResult) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Console
		{
			ColumnDefInt ColDefId(L"Id");
			ColumnDefBin ColDefOutput(L"Output", DA_MAXLEN_OF_CMDOUTPUT);
			TableDef TabDefConsole(L"Console", DA_MAXNUM_OF_RESULT);
			TabDefConsole.AddColumnDef(&ColDefId);
			TabDefConsole.AddColumnDef(&ColDefOutput);
			if (CreateTable(&TabDefConsole) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// TIMESERIES
		{
			for (int Loop = 0; Loop < 5; Loop++) {
				ColumnDefInt ColDefAgentId(L"AgentId");
				ColumnDefBin ColDefUpdTime(L"UpdTime", DA_MAXLEN_OF_UNIXTIME);
				ColumnDefInt ColDefStatus(L"Status");
				ColumnDefInt ColDefSaInterval(L"SaInterval");
				wchar_t TableNameBuf[TABLE_NAME_SIZE];
				StkPlSwPrintf(TableNameBuf, TABLE_NAME_SIZE, L"TimeSeries%d", Loop);
				TableDef TabDefTimeSeries(TableNameBuf, DA_MAXNUM_OF_TIMESERIESDATA);
				TabDefTimeSeries.AddColumnDef(&ColDefAgentId);
				TabDefTimeSeries.AddColumnDef(&ColDefUpdTime);
				TabDefTimeSeries.AddColumnDef(&ColDefStatus);
				TabDefTimeSeries.AddColumnDef(&ColDefSaInterval);
				if (CreateTable(&TabDefTimeSeries) != 0) {
					UnlockAllTable();
					return -1;
				}
			}
		}
		UnlockAllTable();

		// Record for server info table
		{
			ColumnData *ColDatSvr[4];
			ColDatSvr[0] = new ColumnDataInt(L"Id", 0);
			ColDatSvr[1] = new ColumnDataInt(L"PInterval", 30);
			ColDatSvr[2] = new ColumnDataInt(L"SaInterval", 900);
			ColDatSvr[3] = new ColumnDataInt(L"MaxCommandId", 1);
			RecordData* RecSvrInfo = new RecordData(L"ServerInfo", ColDatSvr, 4);
			// Add record
			LockTable(L"ServerInfo", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecSvrInfo);
			UnlockTable(L"ServerInfo");
			delete RecSvrInfo;
		}
		AutoSave(Buf, 30, true);
		return 0;
	} else {
		LockAllTable(2);
		int Ret = LoadData(Buf);
		UnlockAllTable();

		if (Ret != 0) {
			return -2;
		}
		AutoSave(Buf, 30, true);
		return 1;
	}
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
int DataAccess::SetAgentInfo(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int AgtStatus, long long StatusTime)
{
	long long UpdTime = StkPlGetTime();
	// Record information
	ColumnData *ColDatAgt[11];
	int RetMode = 0;
	if (CheckExistenceOfTargetAgent(AgtName) == false) {
		// Add record
		LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
		int MaxId = StkWebAppUm_GetPropertyValueInt(L"MaxAgentId");
		ColDatAgt[0] = new ColumnDataInt(L"Id", MaxId);
		ColDatAgt[1] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[2] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[3] = new ColumnDataInt(L"StatusCmd", -1);
		ColDatAgt[4] = new ColumnDataInt(L"OpStatus", -1);
		ColDatAgt[5] = new ColumnDataInt(L"OpCmd", -1);
		ColDatAgt[6] = new ColumnDataBin(L"ReqTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[7] = new ColumnDataBin(L"AcqTime", (unsigned char*)&StatusTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[8] = new ColumnDataBin(L"UpdTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[9] = new ColumnDataBin(L"IniTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[10] = new ColumnDataBin(L"OpeTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 11);

		int Ret = InsertRecord(RecDatAgt);
		delete RecDatAgt;
		StkWebAppUm_SetPropertyValueInt(L"MaxAgentId", ++MaxId);
		UnlockTable(L"AgentInfo");
		RetMode = 0;
	} else {
		// Update record
		ColumnData *ColDatAgtFind[1];
		ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
		RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

		ColDatAgt[0] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[1] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[2] = new ColumnDataBin(L"AcqTime", (unsigned char*)&StatusTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[3] = new ColumnDataBin(L"UpdTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);

		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 4);

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

	ColumnData *ColDatAgt[4];
	ColDatAgt[0] = new ColumnDataInt(L"StatusCmd", StatusCmd);
	ColDatAgt[1] = new ColumnDataInt(L"Status", -982);
	ColDatAgt[2] = new ColumnDataBin(L"UpdTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
	ColDatAgt[3] = new ColumnDataBin(L"AcqTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 4);

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

	ColumnData *ColDatAgt[3];
	ColDatAgt[0] = new ColumnDataInt(L"OpCmd", OpCmd);
	ColDatAgt[1] = new ColumnDataInt(L"OpStatus", -983);
	ColDatAgt[2] = new ColumnDataBin(L"OpeTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 3);

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

// Type : 0..."ReqTime" update
// Type : 1..."OpeTime" update
// Type : 2..."OpeTime" clear
long long DataAccess::SetAgentInfoForTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME], int Type)
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	long long TmpTime = StkPlGetTime();
	unsigned char TmpTimeBin[DA_MAXLEN_OF_UNIXTIME] = "\0\0\0\0\0\0\0";
	ColumnData *ColDatAgt[1];
	if (Type == 0) {
		StkPlMemCpy((void*)TmpTimeBin, (void*)&TmpTime, 8);
		ColDatAgt[0] = new ColumnDataBin(L"ReqTime", TmpTimeBin, DA_MAXLEN_OF_UNIXTIME);
	} else if (Type == 1) {
		StkPlMemCpy((void*)TmpTimeBin, (void*)&TmpTime, 8);
		ColDatAgt[0] = new ColumnDataBin(L"OpeTime", TmpTimeBin, DA_MAXLEN_OF_UNIXTIME);
	} else if (Type == 2) {
		ColDatAgt[0] = new ColumnDataBin(L"OpeTime", TmpTimeBin, DA_MAXLEN_OF_UNIXTIME);
	}
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
	return TmpTime;
}

int DataAccess::GetAgentInfo(wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
							int AgtStatus[DA_MAXNUM_OF_AGTRECORDS],
							int AgtStatusCmd[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpStatus[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpCmd[DA_MAXNUM_OF_AGTRECORDS],
							long long ReqTime[DA_MAXNUM_OF_AGTRECORDS],
							long long AcqTime[DA_MAXNUM_OF_AGTRECORDS],
							long long UpdTime[DA_MAXNUM_OF_AGTRECORDS],
							long long IniTime[DA_MAXNUM_OF_AGTRECORDS],
	                        long long OpeTime[DA_MAXNUM_OF_AGTRECORDS])
{
	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"AgentInfo");
	UnlockTable(L"AgentInfo");

	int NumOfRec = 0;
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataInt* ColDatId = (ColumnDataInt*)CurrRecDat->GetColumn(0);
		ColumnDataWStr* ColDatName = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
		ColumnDataInt* ColDatStatus = (ColumnDataInt*)CurrRecDat->GetColumn(2);
		ColumnDataInt* ColDatStatusCmd = (ColumnDataInt*)CurrRecDat->GetColumn(3);
		ColumnDataInt* ColDatOpStatus = (ColumnDataInt*)CurrRecDat->GetColumn(4);
		ColumnDataInt* ColDatOpCmd = (ColumnDataInt*)CurrRecDat->GetColumn(5);
		ColumnDataBin* ColDatReqTime = (ColumnDataBin*)CurrRecDat->GetColumn(6);
		ColumnDataBin* ColDatAcqTime = (ColumnDataBin*)CurrRecDat->GetColumn(7);
		ColumnDataBin* ColDatUpdTime = (ColumnDataBin*)CurrRecDat->GetColumn(8);
		ColumnDataBin* ColDatIniTime = (ColumnDataBin*)CurrRecDat->GetColumn(9);
		ColumnDataBin* ColDatOpeTime = (ColumnDataBin*)CurrRecDat->GetColumn(10);
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
		if (ColDatReqTime != NULL && ColDatReqTime->GetValue() != NULL) {
			StkPlMemCpy(&ReqTime[NumOfRec], ColDatReqTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
		}
		if (ColDatAcqTime != NULL && ColDatAcqTime->GetValue() != NULL) {
			StkPlMemCpy(&AcqTime[NumOfRec], ColDatAcqTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
		}
		if (ColDatUpdTime != NULL && ColDatUpdTime->GetValue() != NULL) {
			StkPlMemCpy(&UpdTime[NumOfRec], ColDatUpdTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
		}
		if (ColDatIniTime != NULL && ColDatIniTime->GetValue() != NULL) {
			StkPlMemCpy(&IniTime[NumOfRec], ColDatIniTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
		}
		if (ColDatOpeTime != NULL && ColDatOpeTime->GetValue() != NULL) {
			StkPlMemCpy(&OpeTime[NumOfRec], ColDatOpeTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
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
		ColumnDataInt* ColDatOpCmd = (ColumnDataInt*)RecDatAgt->GetColumn(5);
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
		ColumnDataInt* ColDatOpStatus = (ColumnDataInt*)RecDatAgt->GetColumn(4);
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
	return GetAgentInfoForTime(6, AgtName);;
}

long long DataAccess::GetAgentInfoForUpdTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	return GetAgentInfoForTime(8, AgtName);
}

long long DataAccess::GetAgentInfoForIniTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	return GetAgentInfoForTime(9, AgtName);
}

long long DataAccess::GetAgentInfoForTime(int Type, wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RecDatAgt = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");

	long long IniTime = -1;
	if (RecDatAgt != NULL) {
		ColumnDataBin* ColDatIniTime = (ColumnDataBin*)RecDatAgt->GetColumn(Type);
		if (ColDatIniTime != NULL) {
			long long *IniTimePtr = (long long*)ColDatIniTime->GetValue();
			IniTime = *IniTimePtr;
		}
	}

	delete RecDatAgtFind;
	delete RecDatAgt;

	return IniTime;
}

// AgtName [In] : Delete target agent name
// Return : true=AgentInfo is found, false=not found
bool DataAccess::DeleteAgentInfo(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	{
		// Delete time series data and its mapping info
		ColumnData *ColDatAgtFind[1];
		ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
		RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);
		LockTable(L"AgentInfo", LOCK_SHARE);
		RecordData* ResDat = GetRecord(RecDatAgtFind);
		UnlockTable(L"AgentInfo");
		int AgtId = -1;
		if (ResDat) {
			ColumnDataInt* AgtIdColDat = (ColumnDataInt*)ResDat->GetColumn(0);
			if (AgtIdColDat) {
				AgtId = AgtIdColDat->GetValue();
			}
		}
		delete ResDat;
		delete RecDatAgtFind;
		if (AgtId == -1) {
			return false;
		}

		wchar_t AgtMapPropName[32] = L"";
		StkPlSwPrintf(AgtMapPropName, 32, L"AgentTsdMap%05d", AgtId);
		int TargetTblIdx = StkWebAppUm_GetPropertyValueInt(AgtMapPropName);
		if (TargetTblIdx != -1) {
			// Acquire time series data
			wchar_t TblName[32] = L"";
			StkPlSwPrintf(TblName, 32, L"TimeSeries%d", TargetTblIdx);
			ColumnData *ColDatAgtDel[4];
			ColDatAgtDel[0] = new ColumnDataInt(L"AgentId", AgtId);
			RecordData* RecDatAgtDel = new RecordData(TblName, ColDatAgtDel, 1);
			LockTable(TblName, LOCK_EXCLUSIVE);
			DeleteRecord(RecDatAgtDel);
			UnlockTable(TblName);
			delete RecDatAgtDel;
		}
		StkWebAppUm_DeleteProperty(AgtMapPropName);
	}

	{
		// Delete AgentInfo
		ColumnData *ColDatAgtFind[1];
		ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
		RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);
		LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
		DeleteRecord(RecDatAgtFind);
		UnlockTable(L"AgentInfo");
		delete RecDatAgtFind;
	}

	{
		// Reset ViewSetting
		for (int Loop = 1; Loop <= 8; Loop++) {
			wchar_t TargetAgent[DA_MAXLEN_OF_AGTNAME] = L"";
			wchar_t PropName[256] = L"";
			StkPlSwPrintf(PropName, 256, L"ViewSetting_d%d", Loop);
			StkWebAppUm_GetPropertyValueWStr(PropName, TargetAgent);
			if (StkPlWcsCmp(TargetAgent, AgtName) == 0) {
				StkWebAppUm_SetPropertyValueWStr(PropName, L"");
			}
		}
	}
	return true;
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

	return Ret;
}

int DataAccess::GetCommand(int Id[DA_MAXNUM_OF_CMDRECORDS], wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME], int Type[DA_MAXNUM_OF_CMDRECORDS], char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT],
	                       wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][5][DA_MAXLEN_OF_SERVERFILENAME], wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][5][DA_MAXLEN_OF_AGENTFILENAME], int Timeout[DA_MAXNUM_OF_CMDRECORDS])
{
	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdRes = GetRecord(L"Command");
	UnlockTable(L"Command");

	int NumOfRec = 0;
	RecordData* CurDat = RecDatCmdRes;
	while (CurDat) {
		ColumnDataWStr* ColDatCmdResServerFileName[5];
		ColumnDataWStr* ColDatCmdResAgentFileName[5];

		ColumnDataInt* ColDatCmdResId = (ColumnDataInt*)CurDat->GetColumn(0);
		ColumnDataWStr* ColDatCmdResName = (ColumnDataWStr*)CurDat->GetColumn(1);
		ColumnDataInt* ColDatCmdResType = (ColumnDataInt*)CurDat->GetColumn(2);
		ColumnDataBin* ColDatCmdResScript = (ColumnDataBin*)CurDat->GetColumn(3);
		for (int Loop = 0; Loop < 5; Loop++) {
			ColDatCmdResServerFileName[Loop] = (ColumnDataWStr*)CurDat->GetColumn(Loop + 4);
			ColDatCmdResAgentFileName[Loop] = (ColumnDataWStr*)CurDat->GetColumn(Loop + 9);
		}
		ColumnDataInt* ColDatCmdTimeout = (ColumnDataInt*)CurDat->GetColumn(14);

		if (ColDatCmdResId == NULL || ColDatCmdResName == NULL || ColDatCmdResType == NULL || ColDatCmdResScript == NULL) {
			break;
		}
		bool NullFlag = false;
		for (int Loop = 0; Loop < 5; Loop++) {
			if (ColDatCmdResServerFileName[Loop] == NULL || ColDatCmdResAgentFileName[Loop] == NULL) {
				NullFlag = true;
				break;
			}
		}
		if (NullFlag) {
			break;
		}
		Id[NumOfRec] = ColDatCmdResId->GetValue();
		StkPlWcsCpy(Name[NumOfRec], DA_MAXLEN_OF_CMDNAME, ColDatCmdResName->GetValue());
		Type[NumOfRec] = ColDatCmdResType->GetValue();
		StkPlMemCpy(Script[NumOfRec], ColDatCmdResScript->GetValue(), DA_MAXLEN_OF_CMDSCRIPT);
		for (int Loop = 0; Loop < 5; Loop++) {
			StkPlWcsCpy(ServerFileName[NumOfRec][Loop], DA_MAXLEN_OF_SERVERFILENAME, ColDatCmdResServerFileName[Loop]->GetValue());
			StkPlWcsCpy(AgentFileName[NumOfRec][Loop], DA_MAXLEN_OF_AGENTFILENAME, ColDatCmdResAgentFileName[Loop]->GetValue());
		}
		Timeout[NumOfRec] = ColDatCmdTimeout->GetValue();

		NumOfRec++;
		CurDat = CurDat->GetNextRecord();
	}

	delete RecDatCmdRes;
	return NumOfRec;
}

int DataAccess::GetNumOfCommand()
{
	return GetNumOfRecords(L"Command");
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
int DataAccess::SetCommand(int Id, wchar_t Name[DA_MAXLEN_OF_CMDNAME], int Type, char Script[DA_MAXLEN_OF_CMDSCRIPT],
							wchar_t ServerFileName[5][DA_MAXLEN_OF_SERVERFILENAME], wchar_t AgentFileName[5][DA_MAXLEN_OF_AGENTFILENAME],
							int Timeout)
{
	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);
	LockTable(L"Command", LOCK_SHARE);
	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	UnlockTable(L"Command");

	ColumnData *ColDatCmd[15];
	ColDatCmd[0] = new ColumnDataInt(L"Id", Id);
	ColDatCmd[1] = new ColumnDataWStr(L"Name", Name);
	ColDatCmd[2] = new ColumnDataInt(L"Type", Type);
	ColDatCmd[3] = new ColumnDataBin(L"Script", (unsigned char*)Script, DA_MAXLEN_OF_CMDSCRIPT);
	for (int Loop = 0; Loop < 5; Loop++) {
		wchar_t TmpServerFileName[256] = L"";
		wchar_t TmpAgentFileName[256] = L"";
		StkPlSwPrintf(TmpServerFileName, 256, L"ServerFileName%d", Loop);
		StkPlSwPrintf(TmpAgentFileName, 256, L"AgentFileName%d", Loop);
		ColDatCmd[4 + Loop] = new ColumnDataWStr(TmpServerFileName, ServerFileName[Loop]);
		ColDatCmd[9 + Loop] = new ColumnDataWStr(TmpAgentFileName, AgentFileName[Loop]);
	}
	ColDatCmd[14] = new ColumnDataInt(L"Timeout", Timeout);
	RecordData* RecDatCmd = new RecordData(L"Command", ColDatCmd, 15);
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
	LockTable(L"Command", LOCK_EXCLUSIVE);
	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);

	ColumnData *ColDatCmdFind[1];
	ColDatCmdFind[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatCmdFind = new RecordData(L"Command", ColDatCmdFind, 1);

	RecordData* RecDatCmdFindRes = GetRecord(RecDatCmdFind);
	if (RecDatCmdFindRes != NULL) {
		delete RecDatCmdFindRes;
		DeleteRecord(RecDatCmdFind);
		delete RecDatCmdFind;

		// Change agent status : begin
		{
			ColumnData* ColDatAgtSaCmdSearch[1];
			ColDatAgtSaCmdSearch[0] = new ColumnDataInt(L"StatusCmd", Id);
			RecordData* RecDatAgtSaCmdSearch = new RecordData(L"AgentInfo", ColDatAgtSaCmdSearch, 1);

			ColumnData* ColDatAgtOpCmdSearch[1];
			ColDatAgtOpCmdSearch[0] = new ColumnDataInt(L"OpCmd", Id);
			RecordData* RecDatAgtOpCmdSearch = new RecordData(L"AgentInfo", ColDatAgtOpCmdSearch, 1);

			RecordData* RecDatAgtSaCmdRes = GetRecord(RecDatAgtSaCmdSearch);
			RecordData* RecDatAgtOpCmdRes = GetRecord(RecDatAgtOpCmdSearch);

			delete RecDatAgtSaCmdSearch;
			delete RecDatAgtOpCmdSearch;

			RecordData* CurRecDatSaCmdRes = RecDatAgtSaCmdRes;
			while (CurRecDatSaCmdRes) {
				int AgtId = ((ColumnDataInt*)CurRecDatSaCmdRes->GetColumn(0))->GetValue();
				ColumnData* ColDatRstTgtAgt[1];
				ColDatRstTgtAgt[0] = new ColumnDataInt(L"Id", AgtId);
				RecordData* RecDatRstTgtAgt = new RecordData(L"AgentInfo", ColDatRstTgtAgt, 1);
				ColumnData* ColDatRstDat[2];
				ColDatRstDat[0] = new ColumnDataInt(L"StatusCmd", -1);
				ColDatRstDat[1] = new ColumnDataInt(L"Status", -982);
				RecordData* RecDatRstDat = new RecordData(L"AgentInfo", ColDatRstDat, 2);
				UpdateRecord(RecDatRstTgtAgt, RecDatRstDat);
				delete RecDatRstTgtAgt;
				delete RecDatRstDat;
				CurRecDatSaCmdRes = CurRecDatSaCmdRes->GetNextRecord();
			}
			delete RecDatAgtSaCmdRes;

			RecordData* CurRecDatOpCmdRes = RecDatAgtOpCmdRes;
			while (CurRecDatOpCmdRes) {
				int AgtId = ((ColumnDataInt*)CurRecDatOpCmdRes->GetColumn(0))->GetValue();
				ColumnData* ColDatRstTgtAgt[1];
				ColDatRstTgtAgt[0] = new ColumnDataInt(L"Id", AgtId);
				RecordData* RecDatRstTgtAgt = new RecordData(L"AgentInfo", ColDatRstTgtAgt, 1);
				ColumnData* ColDatRstDat[3];
				ColDatRstDat[0] = new ColumnDataInt(L"OpCmd", -1);
				ColDatRstDat[1] = new ColumnDataInt(L"OpStatus", -984);
				ColDatRstDat[2] = new ColumnDataBin(L"OpeTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
				RecordData* RecDatRstDat = new RecordData(L"AgentInfo", ColDatRstDat, 3);
				UpdateRecord(RecDatRstTgtAgt, RecDatRstDat);
				delete RecDatRstTgtAgt;
				delete RecDatRstDat;
				CurRecDatOpCmdRes = CurRecDatOpCmdRes->GetNextRecord();
			}
			delete RecDatAgtOpCmdRes;
		}
		// Change agent status : end

		UnlockTable(L"AgentInfo");
		UnlockTable(L"Command");
	} else {
		delete RecDatCmdFind;
		UnlockTable(L"AgentInfo");
		UnlockTable(L"Command");
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

int DataAccess::SetCommandResult(wchar_t* AgentName, wchar_t* CommandName, int Status, int ExitCode, char* Data, size_t DataLength)
{
	LockTable(L"Result", LOCK_EXCLUSIVE);
	LockTable(L"Console", LOCK_EXCLUSIVE);
	int MaxId = StkWebAppUm_GetPropertyValueInt(L"MaxResultId");
	StkWebAppUm_SetPropertyValueInt(L"MaxResultId", ++MaxId);

	ColumnData* ColDatCmdResultSearch[1];
	ColDatCmdResultSearch[0] = new ColumnDataInt(L"Id", MaxId - DA_MAXNUM_OF_RESULT);
	RecordData* RecDatCmdResultSearch = new RecordData(L"Result", ColDatCmdResultSearch, 1);

	char UpdTimeBin[DA_MAXLEN_OF_UNIXTIME];
	long long UpdTime = StkPlGetTime();
	char* PtrUpdTime = (char*)&UpdTime;
	StkPlMemCpy(UpdTimeBin, PtrUpdTime, DA_MAXLEN_OF_UNIXTIME);
	ColumnData* ColDatCmdResult[7];
	ColDatCmdResult[0] = new ColumnDataInt(L"Id", MaxId);
	ColDatCmdResult[1] = new ColumnDataBin(L"UpdTime", (unsigned char*)UpdTimeBin, DA_MAXLEN_OF_UNIXTIME);
	ColDatCmdResult[2] = new ColumnDataInt(L"Type", 0);
	ColDatCmdResult[3] = new ColumnDataWStr(L"CmdName", CommandName);
	ColDatCmdResult[4] = new ColumnDataWStr(L"AgtName", AgentName);
	ColDatCmdResult[5] = new ColumnDataInt(L"Status", Status);
	ColDatCmdResult[6] = new ColumnDataInt(L"ExitCode", ExitCode);
	RecordData* RecDatCmdResult = new RecordData(L"Result", ColDatCmdResult, 7);

	if (MaxId > DA_MAXNUM_OF_RESULT) {
		UpdateRecord(RecDatCmdResultSearch, RecDatCmdResult);
	} else {
		InsertRecord(RecDatCmdResult);
	}

	delete RecDatCmdResult;
	delete RecDatCmdResultSearch;

	ColumnData* ColDatConsoleSearch[1];
	ColDatConsoleSearch[0] = new ColumnDataInt(L"Id", MaxId - DA_MAXNUM_OF_RESULT);
	RecordData* RecDatConsoleSearch = new RecordData(L"Console", ColDatConsoleSearch, 1);

	ColumnData* ColDatConsole[2];
	ColDatConsole[0] = new ColumnDataInt(L"Id", MaxId);
	ColDatConsole[1] = new ColumnDataBin(L"Output", (unsigned char*)Data, (int)DataLength);
	RecordData* RecDatConsole = new RecordData(L"Console", ColDatConsole, 2);

	if (MaxId > DA_MAXNUM_OF_RESULT) {
		UpdateRecord(RecDatConsoleSearch, RecDatConsole);
	} else {
		InsertRecord(RecDatConsole);
	}
	delete RecDatConsole;
	delete RecDatConsoleSearch;

	UnlockTable(L"Console");
	UnlockTable(L"Result");
	return true;
}

int DataAccess::GetCommandResult(wchar_t AgentName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_AGTNAME],
								wchar_t CommandName[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_CMDNAME],
								long long UpdTime[DA_MAXNUM_OF_RESULT],
								int Id[DA_MAXNUM_OF_RESULT],
								int Status[DA_MAXNUM_OF_RESULT],
								int ExitCode[DA_MAXNUM_OF_RESULT])
{
	LockTable(L"Result", LOCK_SHARE);
	RecordData* CmdResult = GetRecord(L"Result");
	UnlockTable(L"Result");
	RecordData* CurRec = CmdResult;
	int Index = 0;
	while (CurRec) {
		ColumnDataWStr* ColAgtName = (ColumnDataWStr*)CurRec->GetColumn(L"AgtName");
		if (!ColAgtName) {
			delete CmdResult;
			return -1;
		}
		StkPlWcsCpy(AgentName[Index], DA_MAXLEN_OF_AGTNAME, ColAgtName->GetValue());

		ColumnDataWStr* ColCmdName = (ColumnDataWStr*)CurRec->GetColumn(L"CmdName");
		if (!ColCmdName) {
			delete CmdResult;
			return -1;
		}
		StkPlWcsCpy(CommandName[Index], DA_MAXLEN_OF_CMDNAME, ColCmdName->GetValue());

		ColumnDataBin*  ColUpdTime = (ColumnDataBin*)CurRec->GetColumn(L"UpdTime");
		if (!ColUpdTime) {
			delete CmdResult;
			return -1;
		}
		long long* PtrUpdTime = (long long*)ColUpdTime->GetValue();
		UpdTime[Index] = (long long)*PtrUpdTime;

		ColumnDataInt* ColId = (ColumnDataInt*)CurRec->GetColumn(L"Id");
		if (!ColId) {
			delete CmdResult;
			return -1;
		}
		Id[Index] = ColId->GetValue();

		ColumnDataInt* ColStatus = (ColumnDataInt*)CurRec->GetColumn(L"Status");
		if (!ColStatus) {
			delete CmdResult;
			return -1;
		}
		Status[Index] = ColStatus->GetValue();

		ColumnDataInt* ColExitCode = (ColumnDataInt*)CurRec->GetColumn(L"ExitCode");
		if (!ColExitCode) {
			delete CmdResult;
			return -1;
		}
		ExitCode[Index] = ColExitCode->GetValue();

		Index++;
		CurRec = CurRec->GetNextRecord();
	}
	delete CmdResult;

	for (int Loop1 = 0; Loop1 < Index; Loop1++) {
		// Search max time
		long long MaxTime = 0;
		int MaxTimeIndex = -1;
		for (int Loop2 = Loop1; Loop2 < Index; Loop2++) {
			if (MaxTime < UpdTime[Loop2]) {
				MaxTime = UpdTime[Loop2];
				MaxTimeIndex = Loop2;
			}
		}
		if (MaxTimeIndex != -1) {
			wchar_t TmpAgentName[DA_MAXLEN_OF_AGTNAME] = L"";
			wchar_t TmpCommandName[DA_MAXLEN_OF_CMDNAME] = L"";
			long long TmpUpdTime = 0;
			int TmpId = 0;
			int TmpStatus = 0;
			int TmpExitCode = 0;
			//
			StkPlWcsCpy(TmpAgentName, DA_MAXLEN_OF_AGTNAME, AgentName[Loop1]);
			StkPlWcsCpy(TmpCommandName, DA_MAXLEN_OF_CMDNAME, CommandName[Loop1]);
			TmpUpdTime = UpdTime[Loop1];
			TmpId = Id[Loop1];
			TmpStatus = Status[Loop1];
			TmpExitCode = ExitCode[Loop1];
			//
			StkPlWcsCpy(AgentName[Loop1], DA_MAXLEN_OF_AGTNAME, AgentName[MaxTimeIndex]);
			StkPlWcsCpy(CommandName[Loop1], DA_MAXLEN_OF_CMDNAME, CommandName[MaxTimeIndex]);
			UpdTime[Loop1] = UpdTime[MaxTimeIndex];
			Id[Loop1] = Id[MaxTimeIndex];
			Status[Loop1] = Status[MaxTimeIndex];
			ExitCode[Loop1] = ExitCode[MaxTimeIndex];
			//
			StkPlWcsCpy(AgentName[MaxTimeIndex], DA_MAXLEN_OF_AGTNAME, TmpAgentName);
			StkPlWcsCpy(CommandName[MaxTimeIndex], DA_MAXLEN_OF_CMDNAME, TmpCommandName);
			UpdTime[MaxTimeIndex] = TmpUpdTime;
			Id[MaxTimeIndex] = TmpId;
			Status[MaxTimeIndex] = TmpStatus;
			ExitCode[MaxTimeIndex] = TmpExitCode;
		}
	}

	return Index;
}

int DataAccess::GetOutput(int ResultId, char* Output)
{
	ColumnData *ColDatCmdResult[1];
	ColDatCmdResult[0] = new ColumnDataInt(L"Id", ResultId);
	RecordData* RecDatCmdResult = new RecordData(L"Console", ColDatCmdResult, 1);

	LockTable(L"Console", LOCK_SHARE);
	RecordData* CmdResult = GetRecord(RecDatCmdResult);
	UnlockTable(L"Console");
	delete RecDatCmdResult;

	if (CmdResult) {
		ColumnDataBin* ColOutput = (ColumnDataBin*)CmdResult->GetColumn(L"Output");
		if (!ColOutput) {
			delete CmdResult;
			return -1;
		}
		int Length = ColOutput->GetLength();
		StkPlMemCpy(Output, ColOutput->GetValue(), Length);
		delete CmdResult;
		return Length;
	} else {
		return -1;
	}
}

int DataAccess::IncreaseId(const wchar_t* Name)
{
	int TargetId = -1;
	LockTable(L"Property", LOCK_EXCLUSIVE);
	ColumnData* ColDatSearch[1];
	ColDatSearch[0] = new ColumnDataWStr(L"Name", Name);
	RecordData* RecDatSearch = new RecordData(L"Property", ColDatSearch, 1);
	RecordData* RecDatFound = GetRecord(RecDatSearch);
	if (RecDatFound) {
		TargetId = (((ColumnDataInt*)RecDatFound->GetColumn(1))->GetValue()) + 1;
		ColumnData* ColDatUpd[1];
		ColDatUpd[0] = new ColumnDataInt(L"ValueInt", TargetId);
		RecordData* RecDatUpd = new RecordData(L"Property", ColDatUpd, 1);
		UpdateRecord(RecDatSearch, RecDatUpd);
		delete RecDatUpd;
	}
	UnlockTable(L"Property");
	delete RecDatSearch;
	delete RecDatFound;
	return TargetId;
}

int DataAccess::GetTotalNumOfTimeSeriesData()
{
	int TotalNum = 0;
	for (int Loop = 0; Loop < 5; Loop++) {
		wchar_t TblName[32] = L"";
		StkPlSwPrintf(TblName, 32, L"TimeSeries%d", Loop);
		TotalNum += GetNumOfRecords(TblName);
	}
	return TotalNum;
}

int DataAccess::GetTimeSeriesDataTableContainsLessAmountOfOfAgents()
{
	wchar_t Name[STKWEBAPPUM_MAXNUM_OF_PROPERTY_RECORDS][STKWEBAPPUM_MAXLEN_OF_PROPERTY_NAME];
	int ValInt[STKWEBAPPUM_MAXNUM_OF_PROPERTY_RECORDS];
	wchar_t ValWStr[STKWEBAPPUM_MAXNUM_OF_PROPERTY_RECORDS][STKWEBAPPUM_MAXLEN_OF_PROPERTY_VALUEWSTR];
	int TblCnt[5] = { 0, 0, 0, 0, 0 };

	int NumProp = StkWebAppUm_GetAllPropertyData(Name, ValInt, ValWStr);
	for (int Loop = 0; Loop < NumProp; Loop++) {
		if (StkPlWcsStr(Name[Loop], L"AgentTsdMap") != 0) {
			int TmpInt = ValInt[Loop];
			if (TmpInt >= 0 && TmpInt < 5) {
				TblCnt[TmpInt]++;
			}
		}
	}
	int MinVal = TblCnt[0];
	int MinIndex = 0;
	for (int Loop = 1; Loop < 5; Loop++) {
		if (TblCnt[Loop] < MinVal) {
			MinVal = TblCnt[Loop];
			MinIndex = Loop;
		}
	}
	return MinIndex;
}

int DataAccess::DeleteExpiredTimeSeriesData()
{
	int TotalNumOfTargetIds = 0;
	for (int Loop = 0; Loop < 5; Loop++) {
		wchar_t TblName[32] = L"";
		StkPlSwPrintf(TblName, 32, L"TimeSeries%d", Loop);
		int TargetIdList[DA_MAXNUM_OF_TIMESERIESDATA];
		long long TargetUpdTimeList[DA_MAXNUM_OF_TIMESERIESDATA];
		int NumOfTargetIds = 0;

		// Get target IDs
		long long TargetTime = StkPlGetTime() - (2 * 24 * 60 * 60 + 60 * 60);
		LockTable(TblName, LOCK_SHARE);
		RecordData* ResDat = GetRecord(TblName);
		UnlockTable(TblName);
		RecordData* TopResDat = ResDat;
		while (ResDat) {
			ColumnDataBin* UpdTime = (ColumnDataBin*)ResDat->GetColumn(1);
			if (UpdTime != NULL) {
				long long UpdTimeVal = 0;
				StkPlMemCpy(&UpdTimeVal, UpdTime->GetValue(), DA_MAXLEN_OF_UNIXTIME);
				if (UpdTimeVal < TargetTime) {
					ColumnDataInt* TargetId = (ColumnDataInt*)ResDat->GetColumn(0);
					if (TargetId != NULL) {
						TargetIdList[NumOfTargetIds] = TargetId->GetValue();
						TargetUpdTimeList[NumOfTargetIds] = UpdTimeVal;
						NumOfTargetIds++;
					}
				}
			}
			ResDat = ResDat->GetNextRecord();
		}
		delete TopResDat;

		// Delete expired data
		for (int DelLoop = 0; DelLoop < NumOfTargetIds; DelLoop++) {
			ColumnData *ColDatFind[2];
			ColDatFind[0] = new ColumnDataInt(L"AgentId", TargetIdList[DelLoop]);
			ColDatFind[1] = new ColumnDataBin(L"UpdTime", (unsigned char*)&TargetUpdTimeList[DelLoop], DA_MAXLEN_OF_UNIXTIME);
			RecordData* RecDatFind = new RecordData(TblName, ColDatFind, 2);
			LockTable(TblName, LOCK_EXCLUSIVE);
			DeleteRecord(RecDatFind);
			UnlockTable(TblName);
			delete RecDatFind;
		}

		TotalNumOfTargetIds += NumOfTargetIds;
	}
	return TotalNumOfTargetIds;
}

int DataAccess::AddTimeSeriesData(const wchar_t* AgtName, int Status)
{
	// Retrieve agent ID
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);
	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* ResDat = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");
	int AgtId = -1;
	if (ResDat) {
		ColumnDataInt* AgtIdColDat = (ColumnDataInt*)ResDat->GetColumn(0);
		if (AgtIdColDat) {
			AgtId = AgtIdColDat->GetValue();
		}
	}
	delete ResDat;
	delete RecDatAgtFind;

	// Acquire server information
	int PInterval = -1;
	int SaInterval = -1;
	GetServerInfo(&PInterval, &SaInterval);

	// Add time series data
	if (AgtId == -1) {
		return -1;
	}
	long long UpdTime = StkPlGetTime();
	// Get target TimeSeries table index
	wchar_t AgtMapPropName[32] = L"";
	StkPlSwPrintf(AgtMapPropName, 32, L"AgentTsdMap%05d", AgtId);
	int TargetTblIdx = StkWebAppUm_GetPropertyValueInt(AgtMapPropName);
	if (TargetTblIdx == -1) {
		TargetTblIdx = GetTimeSeriesDataTableContainsLessAmountOfOfAgents();
		StkWebAppUm_SetPropertyValueInt(AgtMapPropName, TargetTblIdx);
	}
	// Acquire time series data
	wchar_t TblName[32] = L"";
	StkPlSwPrintf(TblName, 32, L"TimeSeries%d", TargetTblIdx);
	ColumnData *ColDatAgtUpd[4];
	ColDatAgtUpd[0] = new ColumnDataInt(L"AgentId", AgtId);
	ColDatAgtUpd[1] = new ColumnDataBin(L"UpdTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);
	ColDatAgtUpd[2] = new ColumnDataInt(L"Status", Status);
	ColDatAgtUpd[3] = new ColumnDataInt(L"SaInterval", SaInterval);
	RecordData* RecDatAgtUpd = new RecordData(TblName, ColDatAgtUpd, 4);
	LockTable(TblName, LOCK_EXCLUSIVE);
	InsertRecord(RecDatAgtUpd);
	UnlockTable(TblName);
	delete RecDatAgtUpd;

	return TargetTblIdx;
}

int DataAccess::GetTimeSeriesData(const wchar_t* AgtName, int AgtId[DA_MAXNUM_OF_TIMESERIESDATA], long long UpdTime[DA_MAXNUM_OF_TIMESERIESDATA], int Status[DA_MAXNUM_OF_TIMESERIESDATA], int SaInterval[DA_MAXNUM_OF_TIMESERIESDATA])
{
	// Retrieve agent ID
	ColumnData* ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);
	LockTable(L"AgentInfo", LOCK_SHARE);
	RecordData* RespDat = GetRecord(RecDatAgtFind);
	UnlockTable(L"AgentInfo");
	int TargetAgtId = -1;
	if (RespDat) {
		ColumnDataInt* AgtIdColDat = (ColumnDataInt*)RespDat->GetColumn(0);
		if (AgtIdColDat) {
			TargetAgtId = AgtIdColDat->GetValue();
		}
	}
	delete RespDat;
	delete RecDatAgtFind;

	// Get time series data
	if (TargetAgtId == -1) {
		return -1;
	}
	// Get target TimeSeries table index
	wchar_t AgtMapPropName[32] = L"";
	StkPlSwPrintf(AgtMapPropName, 32, L"AgentTsdMap%05d", TargetAgtId);
	int TargetTblIdx = StkWebAppUm_GetPropertyValueInt(AgtMapPropName);
	if (TargetTblIdx == -1) {
		return 0;
	}

	wchar_t TblName[32] = L"";
	StkPlSwPrintf(TblName, 32, L"TimeSeries%d", TargetTblIdx);
	ColumnData* ColDatTimeSeriesFind[1];
	ColDatTimeSeriesFind[0] = new ColumnDataInt(L"AgentId", TargetAgtId);
	RecordData* RecDatTimeSeriesFind = new RecordData(TblName, ColDatTimeSeriesFind, 1);
	LockTable(TblName, LOCK_SHARE);
	RecordData* ResDat = GetRecord(RecDatTimeSeriesFind);
	UnlockTable(TblName);

	int Index = 0;
	RecordData* CurResDat = ResDat;
	while (CurResDat) {
		ColumnDataInt* AgtIdCol = (ColumnDataInt*)CurResDat->GetColumn(0);
		if (AgtIdCol != NULL) {
			AgtId[Index] = AgtIdCol->GetValue();
		}
		ColumnDataBin* UpdTimeCol = (ColumnDataBin*)CurResDat->GetColumn(1);
		if (UpdTimeCol != NULL) {
			StkPlMemCpy(&UpdTime[Index], UpdTimeCol->GetValue(), DA_MAXLEN_OF_UNIXTIME);
		}
		ColumnDataInt* StatusCol = (ColumnDataInt*)CurResDat->GetColumn(2);
		if (StatusCol != NULL) {
			Status[Index] = StatusCol->GetValue();
		}
		ColumnDataInt* SaIntervalCol = (ColumnDataInt*)CurResDat->GetColumn(3);
		if (SaIntervalCol != NULL) {
			SaInterval[Index] = SaIntervalCol->GetValue();
		}
		CurResDat = CurResDat->GetNextRecord();

		Index++;
	}
	delete RecDatTimeSeriesFind;
	delete ResDat;

	// Sorting
	for (int Loop1 = 0; Loop1 < Index; Loop1++) {
		// Search max time
		long long MinTime = 0x7fffffffffffffff;
		int MinTimeIndex = -1;
		for (int Loop2 = Loop1; Loop2 < Index; Loop2++) {
			if (MinTime > UpdTime[Loop2]) {
				MinTime = UpdTime[Loop2];
				MinTimeIndex = Loop2;
			}
		}
		if (MinTimeIndex != -1 && MinTimeIndex != Loop1) {
			//
			int AgtIdTmp = AgtId[Loop1];
			long long UpdTimeTmp = UpdTime[Loop1];
			int StatusTmp = Status[Loop1];
			int SaIntervalTmp = SaInterval[Loop1];
			//
			AgtId[Loop1] = AgtId[MinTimeIndex];
			UpdTime[Loop1] = UpdTime[MinTimeIndex];
			Status[Loop1] = Status[MinTimeIndex];
			SaInterval[Loop1] = SaInterval[MinTimeIndex];
			//
			AgtId[MinTimeIndex] = AgtIdTmp;
			UpdTime[MinTimeIndex] = UpdTimeTmp;
			Status[MinTimeIndex] = StatusTmp;
			SaInterval[MinTimeIndex] = SaIntervalTmp;
		}
	}

	return Index;
}
