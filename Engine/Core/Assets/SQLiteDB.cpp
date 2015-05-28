#include "SQLiteDB.h"

using namespace Core::Assets;

SQLiteDB::SQLiteDB(std::string filepath)
{
	int rc;

	rc = sqlite3_open(filepath.c_str(), &db);
	if (rc)
	{
		auto errorMessage = std::string(sqlite3_errmsg(db));
		sqlite3_close(db);
		Core::Debug->Error("Can't open database: " + errorMessage);
	}
}
			
SQLiteDB::~SQLiteDB()
{
	if (db != nullptr)
		sqlite3_close(db);
}

void SQLiteDB::Sql(std::string query)
{
	Query = std::move(query);
	int rc = sqlite3_prepare_v2(db, Query.c_str(), -1, &stmt, NULL);
	if (rc)
	{
		Debug->Log("SQL error (" + std::to_string(rc) + "): Failed to prepare statement: \n" + Query + "\n" + sqlite3_errmsg(db));
		return;
	}
}
void SQLiteDB::SqlStatement(std::string query)
{
	Sql(query);
	ExecuteAndFree();
}
void SQLiteDB::FreeQuery()
{
	sqlite3_finalize(stmt); 
}
void SQLiteDB::ExecuteAndFree()
{
	FetchRow();
	FreeQuery();
}

bool SQLiteDB::FetchRow()
{
	int rc = sqlite3_step(stmt); 
	
	if (rc == SQLITE_DONE)
		return false;
	else if (rc == SQLITE_ROW)
		return true;

	Debug->Log("SQL error (" + std::to_string(rc) + "): Failed to step statement: \n" + Query + "\n" + sqlite3_errmsg(db));
	return false;
}

bool SQLiteDB::GetColumnBool(int column)
{
	return (sqlite3_column_int(stmt, column) != 0);
}
int SQLiteDB::GetColumnInt(int column)
{
	return sqlite3_column_int(stmt, column);
}
double SQLiteDB::GetColumnDouble(int column)
{
	return sqlite3_column_double(stmt, column);
}
std::string SQLiteDB::GetColumnString(int column)
{
	auto result = sqlite3_column_text(stmt, column);
	return result != nullptr ? std::string(reinterpret_cast<const char*>(result)) : "";
}
std::unique_ptr<Blob> SQLiteDB::GetColumnBlob(int column)
{
	auto blob = std::make_unique<Blob>(sqlite3_column_bytes(stmt, column));
	std::memcpy(blob->GetData(), sqlite3_column_blob(stmt, column), blob->GetSize());

	// Uncompress blob
	size_t largeSize;
	std::memcpy(&largeSize, blob->GetData() + blob->GetSize() - sizeof(size_t), sizeof(size_t));
	auto largeBlob = std::make_unique<Blob>(largeSize);
	unsigned long finalSize;
	uncompress(largeBlob->GetData(), &finalSize, blob->GetData(), (unsigned long)(blob->GetSize() - sizeof(size_t)));

	return std::move(largeBlob);
}

void SQLiteDB::Bind(int position, bool value)
{
	sqlite3_bind_int(stmt, position, value);
}
void SQLiteDB::Bind(int position, int value)
{
	sqlite3_bind_int(stmt, position, value);
}
void SQLiteDB::Bind(int position, double value)
{
	sqlite3_bind_double(stmt, position, value);
}
void SQLiteDB::Bind(int position, const std::string& value)
{
	sqlite3_bind_text(stmt, position, value.c_str(), -1, SQLITE_TRANSIENT);
}
void SQLiteDB::Bind(int position, const Blob& value)
{
	// Compress the blob
	auto blobSize = value.GetSize();
	auto smallBlob = std::make_unique<Blob>((size_t)compressBound((unsigned long)blobSize)+sizeof(blobSize));
	unsigned long csize = (unsigned long)(smallBlob->GetSize());
	int rs = compress(smallBlob->GetData(), &csize, value.GetData(), (unsigned long)(blobSize));
	if (rs != Z_OK)
	{
		Debug->Error("Error compressing blob: " + rs);
	}
	std::memcpy(smallBlob->GetData() + csize, &blobSize, sizeof(blobSize));
	csize += sizeof(blobSize);	

	// Store compressed blob
	sqlite3_bind_blob(stmt, position, smallBlob->GetData(), csize, SQLITE_TRANSIENT);
}
