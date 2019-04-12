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

		// OdbcConfig table
		ColumnDefInt ColDefOdbcId(L"OdbcId");
		ColumnDefInt ColDefDbmsType(L"DbmsType");
		ColumnDefWStr ColDefConnStr(L"ConnStr", 256);
		ColumnDefInt ColDefOdbcInit(L"Init");
		TableDef TabDefOdbcConfig(L"OdbcConfig", 50);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcId);
		TabDefOdbcConfig.AddColumnDef(&ColDefDbmsType);
		TabDefOdbcConfig.AddColumnDef(&ColDefConnStr);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcInit);
		if (CreateTable(&TabDefOdbcConfig) != 0) {
			return -1;
		}

		// Log table
		ColumnDefInt ColDefLogId(L"Id");
		ColumnDefWStr ColDefLogTime(L"Time", 32);
		ColumnDefWStr ColDefLogMsgEn(L"MessageEn", 128);
		ColumnDefWStr ColDefLogMsgJa(L"MessageJa", 128);
		TableDef TabDefLog(L"Log", 100);
		TabDefLog.AddColumnDef(&ColDefLogId);
		TabDefLog.AddColumnDef(&ColDefLogTime);
		TabDefLog.AddColumnDef(&ColDefLogMsgEn);
		TabDefLog.AddColumnDef(&ColDefLogMsgJa);
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
