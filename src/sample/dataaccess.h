#pragma once

#define DA_MAXLEN_OF_TIME 32
#define DA_MAXLEN_OF_LOGMSG 256
#define DA_MAXNUM_OF_LOGRECORDS 512
#define DA_MAXLEN_OF_AGTNAME 256
#define DA_MAXNUM_OF_AGTRECORDS 256
#define DA_MAXLEN_OF_CMDSCRIPT 8192
#define DA_MAXLEN_OF_CMDNAME 32
#define DA_MAXNUM_OF_CMDRECORDS 16
#define DA_MAXLEN_OF_SERVERFILENAME 256
#define DA_MAXLEN_OF_AGENTFILENAME 256
#define DA_MAXLEN_OF_BUCKETPATH 256

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

	bool CheckExistenceOfTargetAgent(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	void SetAgentInfo(wchar_t[DA_MAXLEN_OF_AGTNAME], int, wchar_t[DA_MAXLEN_OF_TIME], wchar_t[DA_MAXLEN_OF_TIME]);
	void SetAgentInfoForStatusCmd(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	void SetAgentInfoForOpCmd(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	void SetAgentInfoForOpStatus(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	int GetAgentInfo(wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
		wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
		wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME],
		wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME]);
	int GetAgentInfoForOpCmd(wchar_t[DA_MAXLEN_OF_AGTNAME]);

	int GetServerInfo(int*, int*, wchar_t [DA_MAXLEN_OF_BUCKETPATH]);
	int SetServerInfo(int, int, wchar_t[DA_MAXLEN_OF_BUCKETPATH]);
	int GetMaxCommandId();
	int SetMaxCommandId(int);
	void GetFullPathFromFileName(wchar_t[DA_MAXLEN_OF_SERVERFILENAME], const wchar_t[DA_MAXLEN_OF_SERVERFILENAME]);

	int GetCommand(int[DA_MAXNUM_OF_CMDRECORDS], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME], int[DA_MAXNUM_OF_CMDRECORDS], char[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME]);
	int GetCommandNameById(int, wchar_t[DA_MAXLEN_OF_CMDNAME]);
	int SetCommand(int, wchar_t[DA_MAXLEN_OF_CMDNAME], int, char[DA_MAXLEN_OF_CMDSCRIPT], wchar_t[DA_MAXLEN_OF_SERVERFILENAME], wchar_t[DA_MAXLEN_OF_AGENTFILENAME]);
	int DeleteCommand(int);

	int AddLogMsg(const wchar_t[DA_MAXLEN_OF_LOGMSG]);
	int GetMaxLogId();
	int GetNumOfLogs();
	int GetLogs(wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_TIME], wchar_t[DA_MAXNUM_OF_LOGRECORDS][DA_MAXLEN_OF_LOGMSG]);
	int DeleteOldLogs();
};
