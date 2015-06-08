#pragma once

#include <Game.h>

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class ItemEquipBehavior
			{
			public:
				virtual ~ItemEquipBehavior() {}

				virtual void OnEquip(Character& c) = 0;
				virtual void OnUnequip(Character& c) = 0;
			};

			class ItemStatBonus : public ItemEquipBehavior
			{
			private:
				std::string Stat;
				int Bonus;

				void ApplyBonus(Character& c, int Bonus);
			public:
				ItemStatBonus(std::string stat, int bonus);
				virtual ~ItemStatBonus() {}

				virtual void OnEquip(Character& c);
				virtual void OnUnequip(Character& c);
			};

			class Item
			{
				friend class Game::Components::Generator;

			private:
				std::string Type;
				std::vector<std::unique_ptr<ItemEquipBehavior>> IEBs;
				std::string Name;
				int Level;

				void AddEquipBehavior(std::unique_ptr<ItemEquipBehavior> IEB) { IEBs.push_back(std::move(IEB)); }

			public:
				Item() : Type("NULL"), Name(""), Level(0) {}
				Item(std::string type, std::string name, int level) : Name(name), Type(type), Level(level) {}
				~Item() {}

				const std::string& GetType() const { return Type; }
				int GetLevel() const { return Level; }

				void Equip(Character& c) { for (auto& IEB : IEBs) IEB->OnEquip(c); }
				void Unequip(Character& c) { for (auto& IEB : IEBs) IEB->OnUnequip(c); }

				std::string GetName() { return Name; }
			};
		}
	}
}