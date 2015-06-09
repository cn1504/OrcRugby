#include "Character.h"
#include <Components/Generator.h>
#include <Components/SaveDB.h>
#include "Power.h"

namespace std {
	std::string to_string(const Game::Components::Characters::Character& sb)
	{
		return sb.Name + ", " + sb.Gender + " " + sb.Race + " " + sb.CClass->GetName() +
			"\n" + std::to_string(sb.Stats) +
			"\n" + std::to_string(sb.Inv) +
			"\nPrimary Attack: " + sb.Primary->GetName();
	}
}

using namespace Game::Components::Characters;


Character::Character()	// Generate random character
	: Id(-1), CClass(Game::Generator->PickClass()), 
	Stats(this, CClass->GetStatPriorities()), Inv(this), 
	AI(this),
	Primary(Game::Generator->PickPower(*CClass, Stats.Level.Get(), "Primary"))
{
	// Pick Race and Gender
	Race = "Human";
	Gender = (rand() % 2 == 0) ? "Male" : "Female";
	Name = Game::Generator->PickName(Race, Gender);
	Faction = "Player";
}

/*
Character::Character(int id)
	: Id(id), Stats(this), Inv(this)
{	
	Game::Save->LoadCharacter(Id, Name, Race, Gender);

	Stats = std::make_unique<StatBlock>(id, this);
	Inv = std::make_unique<Inventory>(id, this);
}
*/

Character::~Character() {}

void Character::Update()
{
	Entity::Update();

	Stats.Update();
	AI.Update();
}

void Character::Save() {
	Game::Save->SaveCharacter(Id, Name, Race, Gender);
	Stats.Save();
	Inv.Save();
}

void Character::Die()
{
	Core::Debug->Log(Name + " has died.");

	GetParent()->RemoveChild(shared_from_this());

	for (auto& f : Game::Factions)
	{
		for (int j = 0; j < (int)f.second.size(); j++)
		{
			if (f.second[j] == shared_from_this())
			{
				std::swap(f.second[j], f.second.back());
				f.second.resize(f.second.size() - 1);
				j--;
			}
		}
	}
}


