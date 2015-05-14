#include "SaveDB.h"

using namespace Game::Components;

SaveDB::SaveDB(std::shared_ptr<Core::Assets::Database> db)
	: db(db)
{
	// Check if new save database file
	db->Sql("SELECT value FROM Stats_INT WHERE key='NewSave'");
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
		db->SqlStatement("INSERT INTO Stats_INT (key, value) VALUES ('NewSave', 0)");
	}
	else
	{
		// Save Database Exists
		Core::Debug->Log("Using existing save.dat file.");
	}
}

SaveDB::~SaveDB() {}
