#include "SaveDB.h"
#include "Characters/Character.h"

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
		db->SqlStatement("CREATE TABLE Tiles([Column] INTEGER, [Row] INTEGER, Tag TEXT, Orientation INTEGER DEFAULT(0), PRIMARY KEY([Row], [Column]))");
		
		db->SqlStatement("CREATE TABLE Characters([Id] INTEGER PRIMARY KEY NOT NULL UNIQUE, Name TEXT, Race TEXT, Gender TEXT, Class TEXT)");
		db->SqlStatement("CREATE TABLE CharacterStatBlocks([Id] INTEGER PRIMARY KEY REFERENCES Characters (Id) ON UPDATE CASCADE, [Level] INTERGER DEFAULT(1), [Exp] INTEGER DEFAULT(0), [CurrentHP] INTEGER DEFAULT(1), [TotalHP] INTEGER DEFAULT(1), [Str] INTEGER DEFAULT(10), [Dex] INTEGER DEFAULT(10), [Con] INTEGER DEFAULT(10), [Int] INTEGER DEFAULT(10), [Wis] INTEGER DEFAULT(10), [Cha] INTEGER DEFAULT(10), [AC] INTEGER DEFAULT(0), [Fort] INTEGER DEFAULT(0), [Ref] INTEGER DEFAULT(0), [Will] INTEGER DEFAULT(0), [Attack] INTEGER DEFAULT(0), [Damage] INTEGER DEFAULT(0), [Speed] REAL DEFAULT(2.0))");
		db->SqlStatement("CREATE TABLE CharacterPowers([Character] INTEGER REFERENCES Characters (Id) ON UPDATE CASCADE, [Power] INTEGER, PRIMARY KEY (Character, Power))");
		db->SqlStatement("CREATE TABLE CharacterItems([Character] INTEGER REFERENCES Characters (Id) ON UPDATE CASCADE, [Slot] INTEGER, [Item] TEXT, [Level] INTEGER, PRIMARY KEY(Character, Slot))");

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


void SaveDB::NewGame()
{
	// Clear Current Game Objects
	db->SqlStatement("DELETE * FROM Tiles");
	db->SqlStatement("DELETE * FROM Characters");
	
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(0, 0, 'City_01')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(0, 1, 'City_02')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(1, 0, 'City_04')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(1, 1, 'City_03')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(1, 2, 'City_05')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(2, 0, 'City_07')");
	db->SqlStatement("INSERT INTO Tiles(Column, Row, Tag) VALUES(2, 1, 'City_06')");

	// Reset Gold
	Set("Gold", 0);

	Set("GameInProgress", true);
}


std::string SaveDB::GetTileTag(const glm::ivec2& position)
{
	if ((position.y <= -6) || (position.x == 0 && position.y < 0))
	{
		return "Ocean_01";
	}

	std::string value = "";
	db->Sql("SELECT Tag FROM Tiles WHERE Column=" + std::to_string(position.x) + " AND Row=" + std::to_string(position.y) + " LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnString(0);
	}
	db->FreeQuery();
	return value;
}
int SaveDB::GetTileOrientation(const glm::ivec2& position)
{
	int value = 0;
	db->Sql("SELECT Orientation FROM Tiles WHERE Column=" + std::to_string(position.x) + " AND Row=" + std::to_string(position.y) + " LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnInt(0);
	}
	db->FreeQuery();
	return value;
}
void SaveDB::SetTile(const glm::ivec2& position, std::string tag, int orientation)
{
	db->SqlStatement("INSERT OR REPLACE INTO Tiles (Column, Row, Tag, Orientation) VALUES (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", '" + tag + "', " + std::to_string(orientation) + ")");
}



std::shared_ptr<Characters::Character> SaveDB::GetCharacter(int id)
{
	auto c = std::make_shared<Characters::Character>();//id);
	return c;
}
void SaveDB::SaveCharacter(int& id, const std::string& name, const std::string& race, const std::string& gender)
{
	if (id >= 0)
	{
		// Update
		db->SqlStatement("UPDATE Characters SET Name='" + name + "', Race='" + race + "', Gender='" + gender + "' WHERE Id=" + std::to_string(id));
	}
	else
	{
		// New 
		db->SqlStatement("INSERT INTO Characters(Name, Race, Gender) VALUES('" + name + "', '" + race + "', '" + gender + "')");
	}
}
void SaveDB::LoadCharacter(const int& id, std::string& name, std::string& race, std::string& gender)
{
	if (id >= 0)
	{
		db->Sql("SELECT Name, Race, Gender FROM Characters WHERE Id=" + std::to_string(id) + " LIMIT 1");
		if (db->FetchRow())
		{
			name = db->GetColumnString(0);
			race = db->GetColumnString(1);
			gender = db->GetColumnString(2);
		}
		db->FreeQuery();
	}
}
void SaveDB::SaveStats(const Characters::StatBlock& sb)
{

}
void SaveDB::LoadStats(Characters::StatBlock& sb)
{

}
void SaveDB::SaveInventory(const Characters::Inventory& inv)
{

}
void SaveDB::LoadInventory(Characters::Inventory& inv)
{

}