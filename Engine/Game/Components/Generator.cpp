#include "Generator.h"

using namespace Game::Components;


std::string Generator::PickName(std::string race, std::string gender)
{
	std::string name = "ERROR";
	db->Sql("SELECT Name FROM Names WHERE Race='" + race + "' AND Gender='" + gender + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		name = db->GetColumnString(0);
		db->FreeQuery();
	}
	return name;
}