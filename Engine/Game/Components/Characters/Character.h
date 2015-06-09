#pragma once

#include <Game.h>
#include <Components/Entity.h>
#include "StatBlock.h"
#include "Inventory.h"
#include "CharacterAI.h"

namespace std {
	std::string to_string(const Game::Components::Characters::Character& sb);
}

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class Character : public Core::Components::Entity
			{
				friend std::string std::to_string(const Game::Components::Characters::Character& sb);

				friend class StatBlock;
				friend class Inventory;
				friend class CharacterAI;

			private:
				int Id;
				std::unique_ptr<Class> CClass;

				std::string Name;
				std::string Race;
				std::string Gender;
				std::string Faction;
				StatBlock Stats;
				Inventory Inv;
				CharacterAI AI;

				std::unique_ptr<Power> Primary;

			public:
				Character();		// Generate new random character
				//Character(int id);	// Load Character from Save
				~Character();

				virtual void Update();

				void Save();
				void Die();

				const Class& GetClass() const { return *CClass; }
				
				StatBlock& GetStats() { return Stats; }
				const StatBlock& GetStats() const { return Stats; }
				Inventory& GetInventory() { return Inv; }
				const Inventory& GetInventory() const { return Inv; }
				CharacterAI& GetAI() { return AI; }
				const CharacterAI& GetAI() const { return AI; }	

				const std::string& GetFaction() const { return Faction; }
				void SetFaction(std::string faction) { Faction = faction; }
				const std::string& GetName() const { return Name; }
				void SetName(std::string name) { Name = name; }
			};
		}
	}
}