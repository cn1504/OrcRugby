#include "StatBlock.h"
#include <Components/Generator.h>
#include <Components/SaveDB.h>

namespace std {
	std::string to_string(const Game::Components::Characters::StatBlock& sb)
	{
		return
			"Str: " + std::to_string(sb.Strength.Get()) + " (" + std::to_string(sb.Strength.GetMod()) +
			"), Dex: " + std::to_string(sb.Dexterity.Get()) + " (" + std::to_string(sb.Dexterity.GetMod()) +
			"), Con: " + std::to_string(sb.Constitution.Get()) + " (" + std::to_string(sb.Constitution.GetMod()) +
			"), Int: " + std::to_string(sb.Intelligence.Get()) + " (" + std::to_string(sb.Intelligence.GetMod()) +
			"), Wis: " + std::to_string(sb.Wisdom.Get()) + " (" + std::to_string(sb.Wisdom.GetMod()) +
			"), Cha: " + std::to_string(sb.Charisma.Get()) + " (" + std::to_string(sb.Charisma.GetMod()) +
			")\nAC: " + std::to_string(sb.GetDefense_AC()) +
			", Fort: " + std::to_string(sb.GetDefense_Fortitude()) +
			", Ref: " + std::to_string(sb.GetDefense_Reflex()) +
			", Will: " + std::to_string(sb.GetDefense_Will()) +
			", HP: " + std::to_string(sb.HP.GetCurrent()) + "/" + std::to_string(sb.HP.GetTotal()) +
			"\nAB: +" + std::to_string(sb.AttackBonus.Get()) + 
			", Dmg: " + std::to_string(sb.WeaponDamageMin.Get() + sb.DamageBonus.Get()) +
			"-" + std::to_string(sb.WeaponDamageMax.Get() + sb.DamageBonus.Get()) +
			", Crit: " + std::to_string(sb.CriticalRange.Get()) + "-20/x" + std::to_string(sb.CriticalMultiplier.Get())
			;
	}
}

using namespace Game::Components::Characters;


Stat::Stat() { Value = 0; }
int Stat::Get() const { return Value; }
int Stat::GetMod() const { return Value / 2 - 5; }
void Stat::Add(int v) { Value += v; }


Defense::Defense() {}
void Defense::SetBasis(Stat* baseStat1, Stat* baseStat2) { BaseStat1 = baseStat1; BaseStat2 = baseStat2; }
int Defense::Get() const { return ((BaseStat1->GetMod() > BaseStat2->GetMod()) ? BaseStat1->GetMod() : BaseStat2->GetMod()) + Stat::Get(); }


HitPoints::HitPoints(StatBlock* sb) : SB(sb) {}
void HitPoints::SetBasis(Stat* baseStat) { BaseStat = baseStat; }
int HitPoints::GetCurrent() const { return Current.Get(); }
int HitPoints::GetTotal() const { return Total.Get() + BaseStat->GetMod() * SB->Level.Get(); }
float HitPoints::GetPercent() const { return (float)Current.Get() / (float)GetTotal(); }
void HitPoints::Damage(int v) { Current.Add(glm::clamp(-v, 0, GetTotal() - GetCurrent())); }
void HitPoints::Heal(int v) { Current.Add(glm::clamp(v, 0, GetTotal() - GetCurrent())); }
void HitPoints::IncreaseTotal(int v) { Total.Add(v); if (v > 0) Heal(v); else Damage(v); }
Stat& HitPoints::GetCurrentStat() { return Current; }
const Stat& HitPoints::GetCurrentStat() const { return Current; }
Stat& HitPoints::GetTotalStat() { return Total; }
const Stat& HitPoints::GetTotalStat() const { return Total; }


StatBlock::StatBlock(Character* c, int* sp)
	: HP(this)
{
	C = c;

	AC.SetBasis(&Dexterity, &Intelligence);
	Fortitude.SetBasis(&Strength, &Constitution);
	Reflex.SetBasis(&Dexterity, &Intelligence);
	Will.SetBasis(&Wisdom, &Charisma);
	HP.SetBasis(&Constitution);
		
	// Generate Stats
	Level.Add(1);
	
	// Distribute Stats by Class Priority
	int rolls[6];
	rolls[0] = Game::Generator->RollStat();
	rolls[1] = Game::Generator->RollStat();
	rolls[2] = Game::Generator->RollStat();
	rolls[3] = Game::Generator->RollStat();
	rolls[4] = Game::Generator->RollStat();
	rolls[5] = Game::Generator->RollStat();
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if ((sp[i] > sp[j]) && (rolls[i] < rolls[j]))
			{
				std::swap(rolls[i], rolls[j]);
			}
		}
	}
	Strength.Add(rolls[0]);
	Dexterity.Add(rolls[1]);
	Constitution.Add(rolls[2]);
	Intelligence.Add(rolls[3]);
	Wisdom.Add(rolls[4]);
	Charisma.Add(rolls[5]);

	auto hd = C->CClass->GetHitDice();
	HP.IncreaseTotal(hd + rand() % hd + 1);
	HP.Heal(HP.GetTotal());

	CriticalRange.Add(20);
	CriticalMultiplier.Add(2);
}
StatBlock::~StatBlock() {}
/*
StatBlock::StatBlock(int id, Character* c)
	: Strength(std::make_unique<Stat>(Str)), Dexterity(std::make_unique<Stat>(Dex)), Constitution(std::make_unique<Stat>(Con)),
	Intelligence(std::make_unique<Stat>(Int)), Wisdom(std::make_unique<Stat>(Wis)), Charisma(std::make_unique<Stat>(Cha)),
	AC(std::make_unique<Defense>(*Dexterity, *Intelligence, 0)),
	Fortitude(std::make_unique<Defense>(*Strength, *Constitution, 0)),
	Reflex(std::make_unique<Defense>(*Dexterity, *Intelligence, 0)),
	Will(std::make_unique<Defense>(*Wisdom, *Charisma, 0))
{
	CClass = std::move(cclass);
	HP = std::make_unique<HitPoints>(currentHP, totalHP);
	Level = level;
}
*/

void StatBlock::Save() { Game::Save->SaveStats(*this); }
void StatBlock::Update()
{
	for (int i = 0; i < Buffs.size(); i++)
	{
		if (Buffs[i]->Update())
		{
			std::swap(Buffs[i], Buffs.back());
			Buffs.resize(Buffs.size() - 1);
			i--;
		}
	}
	for (int i = 0; i < Debuffs.size(); i++)
	{
		if (Debuffs[i]->Update())
		{
			std::swap(Debuffs[i], Debuffs.back());
			Debuffs.resize(Debuffs.size() - 1);
			i--;
		}
	}

	// Check if dead
	if (HP.GetCurrent() <= 0)
	{
		C->Die();
	}
}

int StatBlock::GetAttackBonus(std::string basis) const
{
	int value = AttackBonus.Get();
	if (basis == "Str") { value += Strength.GetMod(); }
	else if (basis == "Dex") { value += Dexterity.GetMod(); }
	else if (basis == "Con") { value += Constitution.GetMod(); }
	else if (basis == "Int") { value += Intelligence.GetMod(); }
	else if (basis == "Wis") { value += Wisdom.GetMod(); }
	else if (basis == "Cha") { value += Charisma.GetMod(); }
	return value;
}
int StatBlock::GetDefense_AC() const { return 10 + Level.Get() / 2 + AC.Get() + C->CClass->GetDefenseBonus_AC(); }
int StatBlock::GetDefense_Fortitude() const { return 10 + Level.Get() / 2 + Fortitude.Get() + C->CClass->GetDefenseBonus_Fortitude(); }
int StatBlock::GetDefense_Reflex() const { return 10 + Level.Get() / 2 + Reflex.Get() + C->CClass->GetDefenseBonus_Reflex(); }
int StatBlock::GetDefense_Will() const { return 10 + Level.Get() / 2 + Will.Get() + C->CClass->GetDefenseBonus_Will(); }

int StatBlock::RollWeaponDamage() const
{
	auto min = WeaponDamageMin.Get();
	auto max = WeaponDamageMax.Get();
	auto range = max - min;
	if (range < 1)
		return min;
	return rand() % (range + 1) + min;
}

Stat& StatBlock::GetStat(std::string type)
{
	if (type == "Str") { return Strength; }
	else if (type == "Dex") { return Dexterity; }
	else if (type == "Con") { return Constitution; }
	else if (type == "Int") { return Intelligence; }
	else if (type == "Wis") { return Wisdom; }
	else if (type == "Cha") { return Charisma; }
	else if (type == "AC") { return AC; }
	else if (type == "Fort") { return Fortitude; }
	else if (type == "Ref") { return Reflex; }
	else if (type == "Will") { return Will; }
	else if (type == "HP") { return HP.GetTotalStat(); }
	else if (type == "Speed") { return Speed; }
	else if (type == "WDmgMin") { return WeaponDamageMin; }
	else if (type == "WDmgMax") { return WeaponDamageMax; }
	else if (type == "Attack") { return AttackBonus; }
	else if (type == "Dmg") { return DamageBonus; }
	else if (type == "CritRng") { return CriticalRange; }
	else if (type == "CritMult") { return CriticalMultiplier; }
	return NullStat;
}
