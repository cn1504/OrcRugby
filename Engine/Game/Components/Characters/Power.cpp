#include "Power.h"
#include "Character.h"
#include "CharacterAI.h"

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

void Attack::Execute(Character* evoker, Power* power, Character* target)
{
	// Check if target character is valid
	// auto target = evoker->GetAI().PickTarget(Target, Type);
	
	// Make Attack
	bool success = true;
	if (Basis == "None") {}
	else {
		int attack = evoker->GetStats().GetAttackBonus(Basis);
		int defense = (Vs == "AC") ? target->GetStats().GetDefense_AC() :
			(Vs == "Fort") ? target->GetStats().GetDefense_Fortitude() :
			(Vs == "Ref") ? target->GetStats().GetDefense_Reflex() :
			(Vs == "Will") ? target->GetStats().GetDefense_Will() :
			10;

		// Roll the attack
		int roll = rand() % 20 + 1 + attack;
		success = (roll >= defense);
	}

	if (success)
	{
		for (auto& e : OnHit)
		{
			e->Apply(evoker, power, target);
		}
	}
	else
	{
		for (auto& e : OnMiss)
		{
			e->Apply(evoker, power, target);
		}
	}
}

void AttackEffect::Apply(Character* evoker, Power* power, Character* target) 
{
	for (int i = 0; i < Count; i++)
	{
		for (auto& a : power->Attacks)
		{
			a->Execute(evoker, power, target);
		}
	}
}

void DamageEffect::Apply(Character* evoker, Power* power, Character* target) 
{
	int amount = 0;
	if (Type == "WDmg") { amount += evoker->GetStats().RollWeaponDamage(); }
	else { amount += evoker->GetStats().GetStat(Type).GetMod(); }
	amount = static_cast<int>((float)amount * Multiplier);
	amount += Bonus;

	target->GetStats().HP.Damage(amount);
}

void HealEffect::Apply(Character* evoker, Power* power, Character* target)
{
	int amount = 0;
	if (Type == "WDmg") { amount += evoker->GetStats().RollWeaponDamage(); }
	else { amount += evoker->GetStats().GetStat(Type).GetMod(); }
	amount = static_cast<int>((float)amount * Multiplier);
	amount += Bonus;

	target->GetStats().HP.Heal(amount);
}


void BuffEffect::Apply(Character* evoker, Power* power, Character* target)
{
	target->GetStats().Buffs.push_back(std::make_unique<Buff>(target->GetStats().GetStat(Type), Bonus, Duration));
}

void DebuffEffect::Apply(Character* evoker, Power* power, Character* target)
{
	target->GetStats().Debuffs.push_back(std::make_unique<Buff>(target->GetStats().GetStat(Type), Bonus, Duration));
}