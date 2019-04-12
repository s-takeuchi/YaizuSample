#pragma once

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
};
