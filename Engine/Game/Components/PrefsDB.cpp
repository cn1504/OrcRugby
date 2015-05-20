#include "PrefsDB.h"

using namespace Game::Components;

PrefsDB::PrefsDB(std::shared_ptr<Core::Assets::Database> db)
	: db(db)
{
	// Check if new save database file
	db->Sql("SELECT value FROM Prefs_INT WHERE key='NewFile'");
	if (db->FetchRow() == false)
	{
		// New database file
		db->FreeQuery();

		InitializeDB();
	}
	else
	{
		// Save Database Exists
		Core::Debug->Log("Using existing prefs.dat file.");
	}
}

PrefsDB::~PrefsDB() {}

void PrefsDB::Set(std::string key, int value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Prefs_INT (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void PrefsDB::Set(std::string key, double value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Prefs_REAL (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void PrefsDB::Set(std::string key, float value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Prefs_REAL (key, value) VALUES ('" + key + "', " + std::to_string(value) + ")");
}
void PrefsDB::Set(std::string key, std::string value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Prefs_TEXT (key, value) VALUES ('" + key + "', '" + value + "')");
}
void PrefsDB::Set(std::string key, bool value)
{
	db->SqlStatement("INSERT OR REPLACE INTO Prefs_INT (key, value) VALUES ('" + key + "', " + std::to_string((value ? 1 : 0)) + ")");
}

int PrefsDB::GetInt(std::string key)
{
	int value = 0;
	db->Sql("SELECT value FROM Prefs_INT WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnInt(0);
	}
	db->FreeQuery();
	return value;
}
double PrefsDB::GetDouble(std::string key)
{
	double value = NAN;
	db->Sql("SELECT value FROM Prefs_REAL WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnDouble(0);
	}
	db->FreeQuery();
	return value;
}
float PrefsDB::GetFloat(std::string key)
{
	float value = NAN;
	db->Sql("SELECT value FROM Prefs_REAL WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = static_cast<float>(db->GetColumnDouble(0));
	}
	db->FreeQuery();
	return value;
}
std::string PrefsDB::GetString(std::string key)
{
	std::string value = "Invalid String";
	db->Sql("SELECT value FROM Prefs_TEXT WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnString(0);
	}
	db->FreeQuery();
	return value;
}
bool PrefsDB::GetBool(std::string key)
{
	bool value = 0;
	db->Sql("SELECT value FROM Prefs_INT WHERE key='" + key + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = (db->GetColumnInt(0) != 0);
	}
	db->FreeQuery();
	return value;
}

void PrefsDB::InitializeDB()
{
	Core::Debug->Log("Creating new prefs.dat file.");

	// Create tables
	db->SqlStatement("CREATE TABLE Prefs_INT([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value INT)");
	db->SqlStatement("CREATE TABLE Prefs_REAL([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value REAL)");
	db->SqlStatement("CREATE TABLE Prefs_TEXT([key] TEXT PRIMARY KEY NOT NULL UNIQUE, value TEXT)");

	// Initialize default values
	Set("NewFile", false);
	Set("Version", GAME_VERSION);

	// Window Settings
	auto pm = glfwGetPrimaryMonitor();
	auto vm = glfwGetVideoMode(pm);
	int xpos, ypos;
	glfwGetMonitorPos(pm, &xpos, &ypos);
	Set("MainWindow_PosX", xpos);
	Set("MainWindow_PosY", ypos);
	Set("MainWindow_Width", vm->width);
	Set("MainWindow_Height", vm->height);

	// Control Bindings
	Set("CameraUp", "W");
	Set("CameraDown", "S");
	Set("CameraLeft", "A");
	Set("CameraRight", "D");
	Set("CameraOut", "Scroll Up");
	Set("CameraIn", "Scroll Down");
	Set("CameraOut", "Scroll Up");
	Set("CameraRotateLeft", "Q");
	Set("CameraRotateRight", "E");
}