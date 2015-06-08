#pragma once

#include <Game.h>
#include "Item.h"

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class Class
			{
			private:
				std::string Name;
				int StatPriorities[6];
				int DefenseBonuses[4];
				int HitDice;
				std::vector<std::string> UsableItems;

			public:
				Class(std::string name);
				~Class();

				std::string GetName() const;
				int* GetStatPriorities();

				int GetDefenseBonus_AC() const;
				int GetDefenseBonus_Fortitude() const;
				int GetDefenseBonus_Reflex() const;
				int GetDefenseBonus_Will() const;

				int GetHitDice() const;

				bool CanUseItemType(const std::string& type) const;
			};
		}
	}
}