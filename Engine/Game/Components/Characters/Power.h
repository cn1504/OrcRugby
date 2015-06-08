#pragma once

#include <Game.h>
#include "StatBlock.h"
#include <Patterns/Observer.h>
#include "Class.h"

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class AttackEffect;

			class Effect
			{
			public:
				virtual ~Effect() {}
				virtual void Apply(Character* evoker, Power* power, Character* target) = 0;
			};

			class Attack
			{
				friend class Game::Components::Generator;
				friend class Power;

			private:
				std::string Name;
				std::string Target;
				std::string Type;
				std::string Basis;
				int Bonus;
				std::string Vs;
				std::vector<std::unique_ptr<Effect>> OnHit;
				std::vector<std::unique_ptr<Effect>> OnMiss;

			public:

				void Execute(Character* evoker, Power* power, Character* target);
			};
			
			class Power
			{
				friend class Game::Components::Generator;
				friend class AttackEffect;

			private:
				std::string Name;
				std::vector<std::unique_ptr<Attack>> Attacks;

			public:
				std::string GetName() { return Name; }
				void Use(Character* evoker, Character* target);
			};


			class AttackEffect : public Effect
			{
			private:
				int Count;
				std::string Param;

			public:
				AttackEffect(int count, std::string attack) : Count(count), Param(attack) {}
				virtual void Apply(Character* evoker, Power* power, Character* target);
			};

			class DamageEffect : public Effect
			{
			private:
				std::string Type;
				int Bonus;
				float Multiplier;

			public:
				DamageEffect(std::string type, int bonus, float multiplier) 
					: Type(type), Bonus(bonus), Multiplier(multiplier) {}
				virtual void Apply(Character* evoker, Power* power, Character* target);
			};

			class HealEffect : public Effect
			{
			private:
				std::string Type;
				int Bonus;
				float Multiplier;

			public:
				HealEffect(std::string type, int bonus, float multiplier)
					: Type(type), Bonus(bonus), Multiplier(multiplier) {}
				virtual void Apply(Character* evoker, Power* power, Character* target);
			};

			class BuffEffect : public Effect
			{
			private:
				std::string Type;
				int Bonus;
				float Duration;

			public:
				BuffEffect(std::string type, int bonus, float duration)
					: Type(type), Bonus(bonus), Duration(duration) {}
				virtual void Apply(Character* evoker, Power* power, Character* target);
			};

			class DebuffEffect : public Effect
			{
			private:
				std::string Type;
				int Bonus;
				float Duration;

			public:
				DebuffEffect(std::string type, int bonus, float duration)
					: Type(type), Bonus(bonus), Duration(duration) {}
				virtual void Apply(Character* evoker, Power* power, Character* target);
			};

		}
	}
}