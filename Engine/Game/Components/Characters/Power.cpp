#include "Power.h"
#include "Character.h"
#include "CharacterAI.h"
#include <Components/Log.h>

using namespace Game::Components::Characters;

void Power::Use(Character* evoker, Character* target)
{
	for (auto& a : Attacks)
	{
		if (a->Name == "Primary")
		{
			a->Execute(evoker, this, target);
		}
	}
}
Attack* Power::GetAttack(std::string name)
{
	for (auto& a : Attacks)
	{
		if (a->Name == "Primary")
		{
			return a.get();
		}
	}
	return nullptr;
}

void Attack::Execute(Character* evoker, Power* power, Character* target)
{
	// Check if target character is valid
	// auto target = evoker->GetAI().PickTarget(Target, Type);
	
	// Make Attack
	int success = 1;
	if (Basis == "None") {}
	else {
		int attack = evoker->GetStats().GetAttackBonus(Basis);
		int defense = (Vs == "AC") ? target->GetStats().GetDefense_AC() :
			(Vs == "Fort") ? target->GetStats().GetDefense_Fortitude() :
			(Vs == "Ref") ? target->GetStats().GetDefense_Reflex() :
			(Vs == "Will") ? target->GetStats().GetDefense_Will() :
			10;

		// Roll the attack
		int roll = rand() % 20 + 1;
		int result = roll + attack;
		if (roll >= evoker->GetStats().CriticalRange.Get())
			success = (result >= defense) ? 2 : 1;
		else
			success = (result >= defense) ? 1 : 0;
	}

	if (success > 0)
	{
		for (auto& e : OnHit)
		{
			e->Apply(evoker, power, target, success > 1);
		}
	}
	else
	{
		Game::Log->Post("The attack misses.");
		for (auto& e : OnMiss)
		{
			e->Apply(evoker, power, target, false);
		}
	}
}

void AttackEffect::Apply(Character* evoker, Power* power, Character* target, bool critical)
{
	for (int i = 0; i < Count; i++)
	{
		for (auto& a : power->Attacks)
		{
			if (a->Name == Param)
			{
				auto t = evoker->GetAI().PickTarget(a->GetTarget(), a->GetType());
				if (t != nullptr)
					a->Execute(evoker, power, t);
			}
		}
	}
}

void DamageEffect::Apply(Character* evoker, Power* power, Character* target, bool critical)
{
	int amount = 0;
	int critAmount = 0;

	if (Type == "WDmg") { amount += evoker->GetStats().RollWeaponDamage(); }
	else { amount += evoker->GetStats().GetStat(Type).GetMod(); }
	amount = static_cast<int>((float)amount * Multiplier);
	amount += Bonus;

	if (critical)
	{
		for (int i = 1; i < evoker->GetStats().CriticalMultiplier.Get(); i++)
		{
			if (Type == "WDmg") { critAmount += evoker->GetStats().RollWeaponDamage(); }
			else { critAmount += evoker->GetStats().GetStat(Type).GetMod(); }
			critAmount = static_cast<int>((float)critAmount * Multiplier);
			critAmount += Bonus;
		}
	}

	target->GetStats().HP.Damage(amount + critAmount);
	Game::Log->Post(target->GetName() + ((critAmount > 0) ? " is critically hit for " : " is hit for ") + std::to_string(amount + critAmount) + ".");
}

void HealEffect::Apply(Character* evoker, Power* power, Character* target, bool critical)
{
	int amount = 0;
	int critAmount = 0;

	if (Type == "WDmg") { amount += evoker->GetStats().RollWeaponDamage(); }
	else { amount += evoker->GetStats().GetStat(Type).GetMod(); }
	amount = static_cast<int>((float)amount * Multiplier);
	amount += Bonus;

	if (critical)
	{
		for (int i = 1; i < evoker->GetStats().CriticalMultiplier.Get(); i++)
		{
			if (Type == "WDmg") { critAmount += evoker->GetStats().RollWeaponDamage(); }
			else { critAmount += evoker->GetStats().GetStat(Type).GetMod(); }
			critAmount = static_cast<int>((float)critAmount * Multiplier);
			critAmount += Bonus;
		}
	}

	target->GetStats().HP.Heal(amount + critAmount);
	Game::Log->Post(target->GetName() + ((critAmount > 0) ? " is critically healed for " : " is healed for ") + std::to_string(amount + critAmount) + ".");
}


void BuffEffect::Apply(Character* evoker, Power* power, Character* target, bool critical)
{
	target->GetStats().Buffs.push_back(std::make_unique<Buff>(target->GetStats().GetStat(Type), Bonus, Duration));
}

void DebuffEffect::Apply(Character* evoker, Power* power, Character* target, bool critical)
{
	target->GetStats().Debuffs.push_back(std::make_unique<Buff>(target->GetStats().GetStat(Type), Bonus, Duration));
}
