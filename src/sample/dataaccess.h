#pragma once

#define DA_MAXLEN_OF_TIME 32
#define DA_MAXLEN_OF_LOGMSG 256
#define DA_MAXNUM_OF_LOGRECORDS 512
#define DA_MAXLEN_OF_AGTNAME 256

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
	int StopAutoSave(const wchar_t*);
	int CreateTables(const wchar_t*);

	bool DataAccess::CheckExistenceOfTargetAgent(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	void DataAccess::SetAgentInfo(wchar_t[DA_MAXLEN_OF_AGTNAME], wchar_t[DA_MAXLEN_OF_TIME], wchar_t[DA_MAXLEN_OF_TIME]);

	int AddLogMsg(const wchar_t[DA_MAXLEN_OF_LOGMSG]);
	int GetMaxLogId();
	int GetNumOfLogs();
	int GetLogs(wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG]);
	int DeleteOldLogs();
};
