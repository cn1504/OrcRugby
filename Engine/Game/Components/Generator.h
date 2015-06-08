#pragma once

#include <Game.h>
#include <Assets/Database.h>
#include "Characters/Character.h"
#include <Components/Characters/Item.h>

namespace Game
{
	namespace Components
	{
		class Generator
		{
		private:
			std::shared_ptr<Core::Assets::Database> db;

		public:
			Generator(std::shared_ptr<Core::Assets::Database> db) : db(db) {}

			std::shared_ptr<Characters::Character> GenerateCharacter();
			std::string PickName(std::string race, std::string gender);
			std::unique_ptr<Characters::Item> PickItemOfType(std::string type, int level);
			bool ItemFitsSlot(const Characters::Item& i, std::string slot);

			std::unique_ptr<Characters::Class> PickClass();
			void GetClassAttributes(std::string name, int* stats, int* defenseBonuses, int* hitDice, std::vector<std::string>& usable);
			std::unique_ptr<Characters::Power> PickPower(Characters::Class& cclass, int level, std::string type);
			std::unique_ptr<Characters::Power> GetPower(std::string name);

			int Roll(int sides, int count);
			int RollStat();
		};
	}
}