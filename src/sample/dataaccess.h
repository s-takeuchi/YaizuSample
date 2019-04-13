#pragma once

#define DA_MAXLEN_OF_LOGTIME 32
#define DA_MAXLEN_OF_LOGMSG 256
#define DA_MAXNUM_OF_LOGRECORDS 512

class DataAccess
{
private:
	// This instance
	static DataAccess* ThisInstance;
	DataAccess();
	~DataAccess();

public:
	// Get this instance
	static DataAccess* GetInstance();
	int StopAutoSave(wchar_t*);
	int CreateTables(wchar_t*);

	int AddLogMsg(wchar_t[DA_MAXLEN_OF_LOGMSG]);
	int GetMaxLogId();
	int GetNumOfLogs();
	int GetLogs(wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGTIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGTIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG]);
	int DeleteOldLogs();
};
