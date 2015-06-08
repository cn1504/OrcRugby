#include "Generator.h"
#include "Characters/Power.h"

using namespace Game::Components;


std::string Generator::PickName(std::string race, std::string gender)
{
	std::string name = "ERROR";
	db->Sql("SELECT Name FROM Names WHERE Race='" + race + "' AND Gender='" + gender + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		name = db->GetColumnString(0);
	}
	db->FreeQuery();
	return name;
}


std::unique_ptr<Characters::Class> Generator::PickClass()
{
	auto r = rand() % 3;
	std::string c =
		(r == 0) ? "Warrior" :
		(r == 1) ? "Skirmisher" : "Mystic";
		//(r == 2) ? "Mystic" :
		
	return std::make_unique<Characters::Class>(c);
}
void Generator::GetClassAttributes(std::string name, int* stats, int* defenseBonuses, int* hitDice, std::vector<std::string>& usable)
{
	db->Sql("SELECT Str, Dex, Con, Int, Wis, Cha, AC, Fort, Ref, Will, HitDice FROM Classes WHERE Name='" + name + "' LIMIT 1");
	if (db->FetchRow())
	{
		stats[0] = db->GetColumnInt(0);
		stats[1] = db->GetColumnInt(1);
		stats[2] = db->GetColumnInt(2);
		stats[3] = db->GetColumnInt(3);
		stats[4] = db->GetColumnInt(4);
		stats[5] = db->GetColumnInt(5);
		defenseBonuses[0] = db->GetColumnInt(6);
		defenseBonuses[1] = db->GetColumnInt(7);
		defenseBonuses[2] = db->GetColumnInt(8);
		defenseBonuses[3] = db->GetColumnInt(9);
		*hitDice = db->GetColumnInt(10);
		
		db->FreeQuery();
		db->Sql("SELECT ItemType FROM ClassesUsableItems WHERE Class='" + name + "'");
		while (db->FetchRow())
		{
			usable.push_back(db->GetColumnString(0));
		}
	}
	db->FreeQuery();
}


std::shared_ptr<Characters::Character> Generator::GenerateCharacter()
{
	auto c = std::make_shared<Characters::Character>();
	
	db->Sql("SELECT ClassesUsableItems.ItemType FROM ClassesUsableItems, CharacterEquipmentSlotTypes WHERE ClassesUsableItems.ItemType=CharacterEquipmentSlotTypes.ItemType AND CharacterEquipmentSlotTypes.Slot='MainHand' AND ClassesUsableItems.Class='" + c->GetClass().GetName() + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		auto typeName = db->GetColumnString(0);
		db->FreeQuery();
		c->GetInventory().SetMainHand(PickItemOfType(typeName, rand() % 4));
	}
	else
	{
		db->FreeQuery();
		Core::Debug->Log("GenerateCharacter: No MainHand ItemTypes for class: " + c->GetClass().GetName());
	}

	db->Sql("SELECT ClassesUsableItems.ItemType FROM ClassesUsableItems, CharacterEquipmentSlotTypes WHERE ClassesUsableItems.ItemType=CharacterEquipmentSlotTypes.ItemType AND CharacterEquipmentSlotTypes.Slot='OffHand' AND ClassesUsableItems.Class='" + c->GetClass().GetName() + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		auto it = db->GetColumnString(0);
		db->FreeQuery();
		c->GetInventory().SetOffHand(PickItemOfType(it, rand() % 4));
	}
	else
	{
		db->FreeQuery();
		Core::Debug->Log("GenerateCharacter: No Offhand ItemTypes for class: " + c->GetClass().GetName());
	}

	return c;
}
std::unique_ptr<Characters::Power> Generator::PickPower(Characters::Class& cclass, int level, std::string type)
{
	std::unique_ptr<Characters::Power> result;
	db->Sql("SELECT Name FROM Powers WHERE Class='" + cclass.GetName() + "' AND Level<=" + std::to_string(level) + " AND Type='" + type + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		std::string power = db->GetColumnString(0);
		db->FreeQuery();
		result = GetPower(power);
	}
	else
	{
		db->FreeQuery();
	}
	return result;
}
std::unique_ptr<Characters::Power> Generator::GetPower(std::string name)
{
	auto result = std::make_unique<Characters::Power>();
	result->Name = name;

	db->Sql("SELECT Power, Name, Targets, Effect, Basis, Bonus, Vs FROM PowerAttacks WHERE PowerAttacks.Power='" + name + "'");
	while (db->FetchRow())
	{
		auto a = std::make_unique<Characters::Attack>();
		a->Name = db->GetColumnString(1);
		a->Target = db->GetColumnString(2);
		a->Type = db->GetColumnString(3);
		a->Basis = db->GetColumnString(4);
		a->Bonus = db->GetColumnInt(5);
		a->Vs = db->GetColumnString(6);
		result->Attacks.push_back(std::move(a));
	}
	db->FreeQuery();

	if (result->Attacks.size() < 1)
	{
		Core::Debug->Log("Power has no Attacks: " + name);
		return nullptr;
	}

	for (auto& a : result->Attacks)
	{
		db->Sql("SELECT Effect, ParamInt, ParamString, ParamFloat FROM PowerOnHitEffects WHERE PowerOnHitEffects.Power='" + result->Name + "' AND PowerOnHitEffects.Attack='" + a->Name + "'");
		while (db->FetchRow())
		{
			std::unique_ptr<Characters::Effect> e;
			auto effect = db->GetColumnString(0);
			if (effect == "Damage") { e = std::make_unique<Characters::DamageEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Heal") { e = std::make_unique<Characters::HealEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Buff") { e = std::make_unique<Characters::BuffEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Debuff") { e = std::make_unique<Characters::DebuffEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Attack") { e = std::make_unique<Characters::AttackEffect>(db->GetColumnInt(1), db->GetColumnString(2)); }
			a->OnHit.push_back(std::move(e));
		}
		db->FreeQuery();
		if (a->OnHit.size() < 1)
		{
			Core::Debug->Log("Power has no OnHit effects: " + result->Name);
		}
	}

	for (auto& a : result->Attacks)
	{
		db->Sql("SELECT Effect, ParamInt, ParamString, ParamFloat FROM PowerOnMissEffects WHERE PowerOnMissEffects.Power='" + result->Name + "' AND PowerOnMissEffects.Attack='" + a->Name + "'");
		while (db->FetchRow())
		{
			std::unique_ptr<Characters::Effect> e;
			auto effect = db->GetColumnString(0);
			if (effect == "Damage") { e = std::make_unique<Characters::DamageEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Heal") { e = std::make_unique<Characters::HealEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Buff") { e = std::make_unique<Characters::BuffEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Debuff") { e = std::make_unique<Characters::DebuffEffect>(db->GetColumnString(2), db->GetColumnInt(1), static_cast<float>(db->GetColumnDouble(3))); }
			else if (effect == "Attack") { e = std::make_unique<Characters::AttackEffect>(db->GetColumnInt(1), db->GetColumnString(2)); }
			a->OnMiss.push_back(std::move(e));
		}
		db->FreeQuery();
	}

	return result;
}


std::unique_ptr<Characters::Item> Generator::PickItemOfType(std::string type, int level)
{
	std::unique_ptr<Characters::Item> item;
	db->Sql("SELECT Name FROM Items WHERE Type='" + type + "' ORDER BY RANDOM() LIMIT 1");
	if (db->FetchRow())
	{
		item = std::make_unique<Characters::Item>(type, db->GetColumnString(0), level);
		db->FreeQuery();
		
		// Add item properties
		db->Sql("SELECT Stat, Value, Required FROM ItemStats WHERE Item='" + item->GetName() + "' AND Required<=" + std::to_string(level));
		int required = 0;
		while (db->FetchRow())
		{
			item->AddEquipBehavior(std::make_unique<Characters::ItemStatBonus>(db->GetColumnString(0), db->GetColumnInt(1)));

			auto statreq = db->GetColumnInt(2);
			if (required < statreq)
				required = statreq;
		}
		item->Level = required;
		db->FreeQuery();
	}
	else
	{
		db->FreeQuery();
		Core::Debug->Log("Generator::PickItemOfType - No Items of ItemType: " + type);
	}
	return item;
}


bool Generator::ItemFitsSlot(const Characters::Item& i, std::string slot)
{
	bool result = false;
	db->Sql("SELECT ItemType FROM CharacterEquipmentSlotTypes WHERE Slot='" + slot + "'");
	while (db->FetchRow())
	{
		if (i.GetType() == db->GetColumnString(0))
			result = true;
	}
	db->FreeQuery();
	return result;
}


int Generator::Roll(int sides, int count)
{
	int result = 0;
	for (int i = 0; i < count; i++)
	{
		result += rand() % sides + 1;
	}
	return result;
}


int Generator::RollStat()
{
	// 4d6, drop lowest
	const int DICE_COUNT = 4;
	const int DICE_DROP_COUNT = 1;

	std::vector<int> rolls(DICE_COUNT);
	for (int i = 0; i < DICE_COUNT; i++)
	{
		rolls[i] = Roll(6, 1);
	}
	std::sort(rolls.begin(), rolls.end());
	int result = 0;
	for (int i = DICE_DROP_COUNT; i < DICE_COUNT; i++)
		result += rolls[i];
	return result;
}