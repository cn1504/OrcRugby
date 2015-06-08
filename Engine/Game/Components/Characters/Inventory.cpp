#include "Inventory.h"
#include "Character.h"
#include <Components/SaveDB.h>
#include <Components/Generator.h>


namespace std {
	std::string to_string(const Game::Components::Characters::Inventory& inv)
	{
		std::string r = "Equipment:\n" + ((inv.MainHand) ? "\tMainHand: " + inv.MainHand->GetName() + ((inv.MainHand->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.MainHand->GetLevel()) : "") : "") +
			((inv.OffHand) ? "\n\tOffHand: " + inv.OffHand->GetName() + ((inv.OffHand->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.OffHand->GetLevel()) : "") : "") +
			((inv.Armor) ? "\n\tArmor: " + inv.Armor->GetName() + ((inv.Armor->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.Armor->GetLevel()) : "") : "") +
			((inv.Neck) ? "\n\tNeck: " + inv.Neck->GetName() + ((inv.Neck->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.Neck->GetLevel()) : "") : "") +
			((inv.Ring1) ? "\n\tRing1: " + inv.Ring1->GetName() + ((inv.Ring1->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.Ring1->GetLevel()) : "") : "") +
			((inv.Ring2) ? "\n\tRing2: " + inv.Ring2->GetName() + ((inv.Ring2->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.Ring2->GetLevel()) : "") : "") +
			((inv.Trinket) ? "\n\tTrinket: " + inv.Trinket->GetName() + ((inv.Trinket->GetLevel() > 0) ? ", Lvl " + std::to_string(inv.Trinket->GetLevel()) : "") : "");

		if (inv.Backpack.size() > 0)
		{
			r += "Inventory:";
			for (auto& i : inv.Backpack)
			{
				r += "\n\t" + i->GetName();
			}
		}

		return r;
	}
}


using namespace Game::Components::Characters;

const size_t CAPACITY = 20;


Inventory::Inventory(Character* c)
{
	C = c;
}

void Inventory::Save() { Game::Save->SaveInventory(*this); }

std::unique_ptr<Item> Inventory::SwapItem(std::unique_ptr<Item>& target, std::unique_ptr<Item> item)
{
	std::unique_ptr<Item> rv;
	if (C->CClass->CanUseItemType(item->GetType()))
	{
		if (target)
		{
			target->Unequip(*C);
			rv = std::move(target);
		}
		target = std::move(item);
		target->Equip(*C);
	}
	else
	{
		rv = std::move(item);
	}
	return rv;
}

std::unique_ptr<Item> Inventory::SetMainHand(std::unique_ptr<Item> item) 
{
	if (Game::Generator->ItemFitsSlot(*item, "MainHand"))
	{
		if (item->GetType() != "1HWeapon")
		{
			if (OffHand != nullptr)
			{
				if (Backpack.size() >= CAPACITY)
					return item;

				OffHand->Unequip(*C);
				Backpack.push_back(std::move(OffHand));
			}
		}

		return SwapItem(MainHand, std::move(item));
	}
	return item;
}
std::unique_ptr<Item> Inventory::SetOffHand(std::unique_ptr<Item> item) {
	if (MainHand == nullptr || MainHand->GetType() == "1HWeapon")
		return SwapItem(OffHand, std::move(item));
	return item;
}
std::unique_ptr<Item> Inventory::SetArmor(std::unique_ptr<Item> item) { return SwapItem(Armor, std::move(item)); }
std::unique_ptr<Item> Inventory::SetNeck(std::unique_ptr<Item> item) { return SwapItem(Neck, std::move(item)); }
std::unique_ptr<Item> Inventory::SetRing1(std::unique_ptr<Item> item) { return SwapItem(Ring1, std::move(item)); }
std::unique_ptr<Item> Inventory::SetRing2(std::unique_ptr<Item> item) { return SwapItem(Ring2, std::move(item)); }
std::unique_ptr<Item> Inventory::SetTrinket(std::unique_ptr<Item> item) { return SwapItem(Trinket, std::move(item)); }
