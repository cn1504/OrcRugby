#pragma once

#include <Game.h>
#include "StatBlock.h"
#include <Patterns/Observer.h>
#include "Class.h"

namespace std {
	std::string to_string(const Game::Components::Characters::StatBlock& sb);
}

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class Stat
			{
			private:
				int Value;

			public:
				Stat();

				virtual int Get() const;
				int GetMod() const;
				void Add(int v);
			};

			class Defense : public Stat
			{
			private:
				Stat* BaseStat1;
				Stat* BaseStat2;

			public:
				Defense();
				void SetBasis(Stat* BaseStat1, Stat* BaseStat2);
				virtual int Get() const;
			};

			class HitPoints
			{
			private:
				StatBlock* SB;

				Stat Current;
				Stat Total;
				Stat* BaseStat;

			public:
				HitPoints(StatBlock* sb);

				int GetCurrent() const;
				int GetTotal() const;
				float GetPercent() const;
				void Damage(int v);
				void Heal(int v);
				void IncreaseTotal(int v);
				void SetBasis(Stat* baseStat);
				Stat& GetCurrentStat();
				const Stat& GetCurrentStat() const;
				Stat& GetTotalStat();
				const Stat& GetTotalStat() const;
			};

			class Buff
			{
			private:
				Stat* Affected;
				int Amount;
				float Duration;
				float Elapsed;

			public:
				Buff(Stat& affected, int amount, float duration)
					: Affected(&affected), Amount(amount), Duration(duration), Elapsed(0.0f) {
					Affected->Add(Amount);
				}
				~Buff() { Affected->Add(-Amount); }
				bool Update() { Elapsed += Core::Time->Delta; return (Elapsed > Duration); }
			};



			class StatBlock : public Core::Patterns::Observable
			{
				friend std::string std::to_string(const Game::Components::Characters::StatBlock& sb);
				
				friend class Character;

				friend class HitPoints;
				friend class ItemStatBonus;
				
				friend class DamageEffect;
				friend class HealEffect;
				friend class BuffEffect;
				friend class DebuffEffect;
				
			private:
				Stat NullStat;

				Character* C;
				
				Stat Strength;
				Stat Dexterity;
				Stat Constitution;
				Stat Intelligence;
				Stat Wisdom;
				Stat Charisma;

				HitPoints HP;
				Defense AC;
				Defense Fortitude;
				Defense Reflex;
				Defense Will;

				Stat Speed;
				Stat WeaponDamageMin;
				Stat WeaponDamageMax;
				Stat AttackBonus;
				Stat DamageBonus;
				Stat CriticalRange;
				Stat CriticalMultiplier;

				Stat Level;
				Stat Exp;

				std::vector<std::unique_ptr<Buff>> Buffs;
				std::vector<std::unique_ptr<Buff>> Debuffs;

				Stat& GetStat(std::string type);

			public:
				StatBlock(Character* c, int* sp);	// Generate new character
				//StatBlock(int id, Character* c);	// Load existing character
				~StatBlock();
				
				void Save();
				void Update();

				int GetAttackBonus(std::string basis) const;
				int GetDefense_AC() const;
				int GetDefense_Fortitude() const;
				int GetDefense_Reflex() const;
				int GetDefense_Will() const;
				
				int RollWeaponDamage() const;
				void ApplyDamage(int v);
			};
		}
	}
}