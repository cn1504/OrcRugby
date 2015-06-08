#include "Class.h"
#include <Components/Generator.h>

using namespace Game::Components::Characters;


Class::Class(std::string name) { Name = name; Game::Generator->GetClassAttributes(Name, StatPriorities, DefenseBonuses, &HitDice, UsableItems); }
Class::~Class() {}

std::string Class::GetName() const { return Name; }
int* Class::GetStatPriorities() { return StatPriorities; }

int Class::GetDefenseBonus_AC() const { return DefenseBonuses[0]; }
int Class::GetDefenseBonus_Fortitude() const { return DefenseBonuses[1]; }
int Class::GetDefenseBonus_Reflex() const { return DefenseBonuses[2]; }
int Class::GetDefenseBonus_Will() const { return DefenseBonuses[3]; }

int Class::GetHitDice() const { return HitDice; }

bool Class::CanUseItemType(const std::string& type) const
{
	for (auto& it : UsableItems)
	{
		if (it == type)
		{
			return true;
		}
	}
	return false;
}

