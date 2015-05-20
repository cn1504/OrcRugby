#include "SaveDB.h"

using namespace Game::Components;

SaveDB::SaveDB(std::shared_ptr<Core::Assets::Database> db)
	: db(db)
{
	// Check if new save database file
	db->Sql("SELECT value FROM Stats_INT WHERE key='NewSave' LIMIT 1");
	if (db->FetchRow() == false)
	{
		// New database file
		db->FreeQuery();

		Core::Debug->Log("Creating new save.dat file.");

		// Create tables
		db->SqlStatement("CREATE TABLE Stats_INT([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value INT)");
		db->SqlStatement("CREATE TABLE Stats_REAL([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value REAL)");
		db->SqlStatement("CREATE TABLE Stats_TEXT([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value TEXT)");

		// Initialize default values
		Set("NewSave", false);
		Set("GameInProgress", false);
		Set("TimeRatio", 1.0 / (300.0 / 86400.0));
	}
	else
	{
		// Save Database Exists
		Core::Debug->Log("Using existing save.dat file.");
	}
}

SaveDB::~SaveDB() {}


void SaveDB::Set(std::string key, int value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Stats_INT (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void SaveDB::Set(std::string key, double value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Stats_REAL (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void SaveDB::Set(std::string key, float value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Stats_REAL (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void SaveDB::Set(std::string key, std::string value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Stats_TEXT (key, value) VALUES ('" + key + "', '" + value + "')");
}

int SaveDB::GetInt(std::string key)
{
	int value = 0;
	db->Sql("SELECT value FROM Stats_INT WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnInt(0);
	}
	db->FreeQuery();
	return value;
}
double SaveDB::GetDouble(std::string key)
{
	double value = NAN;
	db->Sql("SELECT value FROM Stats_REAL WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnDouble(0);
	}
	db->FreeQuery();
	return value;
}
float SaveDB::GetFloat(std::string key)
{
	float value = NAN;
	db->Sql("SELECT value FROM Stats_REAL WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = static_cast<float>(db->GetColumnDouble(0));
	}
	db->FreeQuery();
	return value;
}
std::string SaveDB::GetString(std::string key)
{
	std::string value = "Invalid String";
	db->Sql("SELECT value FROM Stats_TEXT WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnString(0);
	}
	db->FreeQuery();
	return value;
}

