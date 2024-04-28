#include "../../sqlite/sqlite3.h"
#include "IDatabase.h"

class SqliteDatabase : public IDatabase
{
public:
	virtual bool open();	
	virtual bool close();

	// queries
	virtual bool doesUserExist(const std::string& username) const;
	virtual bool doesPasswordMatch(const std::string&username , const std::string_view& password) const;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email);

private:
	sqlite3* _db;

	// function to run queries on the databases
	void runQuery(const std::string_view& query, const sqlite3_callback& callback, void* data) const;

	// callback functions
	static int callbackInt(void* _data, int argc, char** argv, char** columnsNames);
	static int callbackText(void* _data, int argc, char** argv, char** columnsNames);
};
