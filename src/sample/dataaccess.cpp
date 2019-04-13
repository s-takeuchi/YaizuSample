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
int DataAccess::StopAutoSave(wchar_t* DataFileName)
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
int DataAccess::CreateTables(wchar_t* DataFileName)
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

	LockAllTable(2);
	if (StkPlGetFileSize(Buf) == 0) {

		// Log table
		ColumnDefInt ColDefLogId(L"Id");
		ColumnDefWStr ColDefLogTimeUtc(L"TimeUtc", DA_MAXLEN_OF_LOGTIME);
		ColumnDefWStr ColDefLogTimeLocal(L"TimeLocal", DA_MAXLEN_OF_LOGTIME);
		ColumnDefWStr ColDefLogMsg(L"Message", DA_MAXLEN_OF_LOGMSG);
		TableDef TabDefLog(L"Log", DA_MAXNUM_OF_LOGRECORDS);
		TabDefLog.AddColumnDef(&ColDefLogId);
		TabDefLog.AddColumnDef(&ColDefLogTimeUtc);
		TabDefLog.AddColumnDef(&ColDefLogTimeLocal);
		TabDefLog.AddColumnDef(&ColDefLogMsg);
		if (CreateTable(&TabDefLog) != 0) {
			return -1;
		}

	} else {
		if (LoadData(Buf) != 0) {
			UnlockAllTable();
			return -1;
		}
	}
	UnlockAllTable();

	return 0;
}

// Add log message
// LogMsg [in] : Message which you want to insert
// Return : always zero returned.
int DataAccess::AddLogMsg(wchar_t LogMsg[DA_MAXLEN_OF_LOGTIME])
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
	wchar_t LogMsgTimeUtc[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGTIME],
	wchar_t LogMsgTimeLocal[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGTIME],
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
			StkPlSwPrintf(LogMsgTimeUtc[NumOfRec], DA_MAXLEN_OF_LOGTIME, ColDatTimeUtc->GetValue());
		} else {
			StkPlSwPrintf(LogMsgTimeUtc[NumOfRec], DA_MAXLEN_OF_LOGTIME, L"");
		}
		if (ColDatTimeLocal != NULL && ColDatTimeLocal->GetValue() != NULL) {
			StkPlSwPrintf(LogMsgTimeLocal[NumOfRec], DA_MAXLEN_OF_LOGTIME, ColDatTimeLocal->GetValue());
		} else {
			StkPlSwPrintf(LogMsgTimeLocal[NumOfRec], DA_MAXLEN_OF_LOGTIME, L"");
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
