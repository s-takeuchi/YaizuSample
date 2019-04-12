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
};
