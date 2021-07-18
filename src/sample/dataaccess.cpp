#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkdata/stkdata.h"
#include "../../../YaizuComLib/src/stkdata/stkdataapi.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "sample.h"
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
			ColumnDefWStr ColDefAgtName(L"Name", DA_MAXLEN_OF_AGTNAME);
			ColumnDefInt  ColDefAgtStatus(L"Status");
			ColumnDefInt  ColDefAgtStatusCmd(L"StatusCmd");
			ColumnDefInt  ColDefAgtOpStatus(L"OpStatus");
			ColumnDefInt  ColDefAgtOpCmd(L"OpCmd");
			ColumnDefBin  ColDefAgtReqTime(L"ReqTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtAcqTime(L"AcqTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtUpdTime(L"UpdTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefBin  ColDefAgtIniTime(L"IniTime", DA_MAXLEN_OF_UNIXTIME);
			TableDef TabDefAgtInfo(L"AgentInfo", DA_MAXNUM_OF_AGTRECORDS);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtName);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtStatusCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpStatus);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtOpCmd);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtReqTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtAcqTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtUpdTime);
			TabDefAgtInfo.AddColumnDef(&ColDefAgtIniTime);
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
		// Result table
		{
			ColumnDefInt ColDefId(L"Id");
			ColumnDefBin ColDefUpdTime(L"UpdTime", DA_MAXLEN_OF_UNIXTIME);
			ColumnDefInt ColDefType(L"Type");
			ColumnDefWStr ColDefComName(L"CmdName", DA_MAXLEN_OF_CMDNAME);
			ColumnDefWStr ColDefAgtName(L"AgtName", DA_MAXLEN_OF_AGTNAME);
			ColumnDefInt ColDefStatus(L"Status");
			TableDef TabDefResult(L"Result", DA_MAXNUM_OF_RESULT);
			TabDefResult.AddColumnDef(&ColDefId);
			TabDefResult.AddColumnDef(&ColDefUpdTime);
			TabDefResult.AddColumnDef(&ColDefType);
			TabDefResult.AddColumnDef(&ColDefComName);
			TabDefResult.AddColumnDef(&ColDefAgtName);
			TabDefResult.AddColumnDef(&ColDefStatus);
			if (CreateTable(&TabDefResult) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		// Console
		{
			ColumnDefInt ColDefId(L"Id");
			ColumnDefBin ColDefOutput(L"Output", DA_MAXLEN_OF_CMDOUTPUT);
			TableDef TabDefResult(L"Console", DA_MAXNUM_OF_RESULT);
			TabDefResult.AddColumnDef(&ColDefId);
			TabDefResult.AddColumnDef(&ColDefOutput);
			if (CreateTable(&TabDefResult) != 0) {
				UnlockAllTable();
				return -1;
			}
		}
		UnlockAllTable();

		// Record for server info table
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
	ColumnData *ColDatAgt[9];
	int RetMode = 0;
	if (CheckExistenceOfTargetAgent(AgtName) == false) {
		// Add record
		ColDatAgt[0] = new ColumnDataWStr(L"Name", AgtName);
		ColDatAgt[1] = new ColumnDataInt(L"Status", AgtStatus);
		ColDatAgt[2] = new ColumnDataInt(L"StatusCmd", -1);
		ColDatAgt[3] = new ColumnDataInt(L"OpStatus", -1);
		ColDatAgt[4] = new ColumnDataInt(L"OpCmd", -1);
		ColDatAgt[5] = new ColumnDataBin(L"ReqTime", (unsigned char*)"\0\0\0\0\0\0\0\0", DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[6] = new ColumnDataBin(L"AcqTime", (unsigned char*)&StatusTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[7] = new ColumnDataBin(L"UpdTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);
		ColDatAgt[8] = new ColumnDataBin(L"IniTime", (unsigned char*)&UpdTime, DA_MAXLEN_OF_UNIXTIME);
		RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 9);

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

long long DataAccess::SetAgentInfoForReqTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	ColumnData *ColDatAgtFind[1];
	ColDatAgtFind[0] = new ColumnDataWStr(L"Name", AgtName);
	RecordData* RecDatAgtFind = new RecordData(L"AgentInfo", ColDatAgtFind, 1);

	long long ReqTime = StkPlGetTime();
	unsigned char ReqTimeBin[DA_MAXLEN_OF_UNIXTIME] = "\0\0\0\0\0\0\0";
	StkPlMemCpy((void*)ReqTimeBin, (void*)&ReqTime, 8);
	ColumnData *ColDatAgt[1];
	ColDatAgt[0] = new ColumnDataBin(L"ReqTime", ReqTimeBin, DA_MAXLEN_OF_UNIXTIME);
	RecordData* RecDatAgt = new RecordData(L"AgentInfo", ColDatAgt, 1);

	LockTable(L"AgentInfo", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatAgtFind, RecDatAgt);
	UnlockTable(L"AgentInfo");
	delete RecDatAgt;
	delete RecDatAgtFind;
	return ReqTime;
}

int DataAccess::GetAgentInfo(wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
							int AgtStatus[DA_MAXNUM_OF_AGTRECORDS],
							int AgtStatusCmd[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpStatus[DA_MAXNUM_OF_AGTRECORDS],
	                        int OpCmd[DA_MAXNUM_OF_AGTRECORDS],
							long long ReqTime[DA_MAXNUM_OF_AGTRECORDS],
							long long AcqTime[DA_MAXNUM_OF_AGTRECORDS],
							long long UpdTime[DA_MAXNUM_OF_AGTRECORDS],
							long long IniTime[DA_MAXNUM_OF_AGTRECORDS])
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
		ColumnDataBin* ColDatAcqTime = (ColumnDataBin*)CurrRecDat->GetColumn(6);
		ColumnDataBin* ColDatUpdTime = (ColumnDataBin*)CurrRecDat->GetColumn(7);
		ColumnDataBin* ColDatIniTime = (ColumnDataBin*)CurrRecDat->GetColumn(8);
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
	return GetAgentInfoForTime(5, AgtName);;
}

long long DataAccess::GetAgentInfoForUpdTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	return GetAgentInfoForTime(7, AgtName);
}

long long DataAccess::GetAgentInfoForIniTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME])
{
	return GetAgentInfoForTime(8, AgtName);
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

int DataAccess::SetCommandResult(wchar_t* AgentName, wchar_t* CommandName, char* Data, size_t DataLength)
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
	ColumnData* ColDatCmdResult[6];
	ColDatCmdResult[0] = new ColumnDataInt(L"Id", MaxId);
	ColDatCmdResult[1] = new ColumnDataBin(L"UpdTime", (unsigned char*)UpdTimeBin, DA_MAXLEN_OF_UNIXTIME);
	ColDatCmdResult[2] = new ColumnDataInt(L"Type", 0);
	ColDatCmdResult[3] = new ColumnDataWStr(L"CmdName", CommandName);
	ColDatCmdResult[4] = new ColumnDataWStr(L"AgtName", AgentName);
	ColDatCmdResult[5] = new ColumnDataInt(L"Status", 0);
	RecordData* RecDatCmdResult = new RecordData(L"Result", ColDatCmdResult, 6);

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
	                             int Id[DA_MAXNUM_OF_RESULT])
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
		StkPlWcsCpy(CommandName[Index], DA_MAXLEN_OF_AGTNAME, ColCmdName->GetValue());

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

		Index++;
		CurRec = CurRec->GetNextRecord();
	}
	delete CmdResult;

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
		if (MinTimeIndex != -1) {
			wchar_t TmpAgentName[DA_MAXLEN_OF_AGTNAME] = L"";
			wchar_t TmpCommandName[DA_MAXLEN_OF_CMDNAME] = L"";
			long long TmpUpdTime = 0;
			int TmpId = 0;
			//
			StkPlWcsCpy(TmpAgentName, DA_MAXLEN_OF_AGTNAME, AgentName[Loop1]);
			StkPlWcsCpy(TmpCommandName, DA_MAXLEN_OF_CMDNAME, CommandName[Loop1]);
			TmpUpdTime = UpdTime[Loop1];
			TmpId = Id[Loop1];
			//
			StkPlWcsCpy(AgentName[Loop1], DA_MAXLEN_OF_AGTNAME, AgentName[MinTimeIndex]);
			StkPlWcsCpy(CommandName[Loop1], DA_MAXLEN_OF_CMDNAME, CommandName[MinTimeIndex]);
			UpdTime[Loop1] = UpdTime[MinTimeIndex];
			Id[Loop1] = Id[MinTimeIndex];
			//
			StkPlWcsCpy(AgentName[MinTimeIndex], DA_MAXLEN_OF_AGTNAME, TmpAgentName);
			StkPlWcsCpy(CommandName[MinTimeIndex], DA_MAXLEN_OF_CMDNAME, TmpCommandName);
			UpdTime[MinTimeIndex] = TmpUpdTime;
			Id[MinTimeIndex] = TmpId;
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

