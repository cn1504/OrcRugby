#pragma once

#include <Game.h>

namespace std {
	std::string to_string(const Game::Components::Characters::Inventory& inv);
}

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class Inventory
			{
				friend std::string std::to_string(const Game::Components::Characters::Inventory& inv);

			private:
				Character* C;

				std::unique_ptr<Item> MainHand;
				std::unique_ptr<Item> OffHand;
				std::unique_ptr<Item> Armor;
				std::unique_ptr<Item> Neck;
				std::unique_ptr<Item> Ring1;
				std::unique_ptr<Item> Ring2;
				std::unique_ptr<Item> Trinket;

				std::vector<std::unique_ptr<Item>> Backpack;

				std::unique_ptr<Item> SwapItem(std::unique_ptr<Item>& target, std::unique_ptr<Item> item);

			public:
				Inventory(Character* c);

				void Save();

				// Equips an item if the character can equip it
				// Returns: on success: previously equipped item,
				//          on failure: the item attempting to be equipped
				std::unique_ptr<Item> SetMainHand(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetOffHand(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetArmor(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetNeck(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetRing1(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetRing2(std::unique_ptr<Item> item);
				std::unique_ptr<Item> SetTrinket(std::unique_ptr<Item> item);
			};
		}
	}
}