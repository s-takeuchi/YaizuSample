#pragma once

#define DA_MAXLEN_OF_TIME 32
#define DA_MAXLEN_OF_UNIXTIME 8
#define DA_MAXLEN_OF_LOGMSG 256
#define DA_MAXNUM_OF_LOGRECORDS 210
#define DA_MAXLEN_OF_AGTNAME 256
#define DA_MAXNUM_OF_AGTRECORDS 256
#define DA_MAXLEN_OF_CMDSCRIPT 8192
#define DA_MAXLEN_OF_CMDNAME 32
#define DA_MAXNUM_OF_CMDRECORDS 16
#define DA_MAXLEN_OF_SERVERFILENAME 256
#define DA_MAXLEN_OF_AGENTFILENAME 256
#define DA_MAXLEN_OF_BUCKETPATH 256
#define DA_MAXLEN_OF_USERNAME 256
#define DA_MAXLEN_OF_PASSWORD 32
#define DA_MAXLEN_OF_TARGETURL 256
#define DA_MAXNUM_OF_USERRECORDS 64
#define DA_MAXNUM_OF_THREADS 305
#define DA_MAXLEN_OF_CMDOUTPUT 262144
#define DA_MAXNUM_OF_RESULT 120
#define DA_MAXNUM_OF_TIMESERIESDATA 16383

class DataAccess
{
private:
	// This instance
	static DataAccess* ThisInstance;
	DataAccess();
	~DataAccess();
	long long GetAgentInfoForTime(int, wchar_t[DA_MAXLEN_OF_AGTNAME]);

public:
	// Get this instance
	static DataAccess* GetInstance();
	int StopAutoSave(const wchar_t*);
	int CreateTables(const wchar_t*);

	bool CheckExistenceOfTargetAgent(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	int SetAgentInfo(wchar_t[DA_MAXLEN_OF_AGTNAME], int, long long);
	void SetAgentInfoForStatusCmd(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	void SetAgentInfoForStatus(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	void SetAgentInfoForOpCmd(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	void SetAgentInfoForOpStatus(wchar_t[DA_MAXLEN_OF_AGTNAME], int);
	long long SetAgentInfoForReqTime(wchar_t AgtName[DA_MAXLEN_OF_AGTNAME]);
	int GetAgentInfo(wchar_t[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		int[DA_MAXNUM_OF_AGTRECORDS],
		long long[DA_MAXNUM_OF_AGTRECORDS],
		long long[DA_MAXNUM_OF_AGTRECORDS],
		long long[DA_MAXNUM_OF_AGTRECORDS],
		long long[DA_MAXNUM_OF_AGTRECORDS]);
	int GetAgentInfoForOpCmd(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	int GetAgentInfoForOpStatus(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	long long GetAgentInfoForReqTime(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	long long GetAgentInfoForUpdTime(wchar_t[DA_MAXLEN_OF_AGTNAME]);
	long long GetAgentInfoForIniTime(wchar_t[DA_MAXLEN_OF_AGTNAME]);

	int GetServerInfo(int*, int*);
	int SetServerInfo(int, int);
	int GetMaxCommandId();
	int SetMaxCommandId(int);

	int GetCommand(int[DA_MAXNUM_OF_CMDRECORDS], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME], int[DA_MAXNUM_OF_CMDRECORDS], char[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME], wchar_t[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME]);
	int GetCommandNameById(int, wchar_t[DA_MAXLEN_OF_CMDNAME]);
	bool CheckCommandExistenceByName(wchar_t[DA_MAXLEN_OF_CMDNAME]);
	int SetCommand(int, wchar_t[DA_MAXLEN_OF_CMDNAME], int, char[DA_MAXLEN_OF_CMDSCRIPT], wchar_t[DA_MAXLEN_OF_SERVERFILENAME], wchar_t[DA_MAXLEN_OF_AGENTFILENAME]);
	int DeleteCommand(int);
	int SetCommandResult(wchar_t*, wchar_t*, char*, size_t);
	int GetCommandResult(wchar_t[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_AGTNAME], wchar_t[DA_MAXNUM_OF_RESULT][DA_MAXLEN_OF_CMDNAME], long long[DA_MAXNUM_OF_RESULT], int[DA_MAXNUM_OF_RESULT]);
	int GetOutput(int, char*);

	int IncreaseId(const wchar_t*);
};
