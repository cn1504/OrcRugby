#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	class CharacterStats
		: public Component
	{
	public:
		CharacterStats(std::istream& in);
		virtual ~CharacterStats();

		virtual void Load();
		virtual void Update();

		void Load(std::istream& in);
		void Save(std::ostream& out);

		// Stats
		int Strength;
		int Dexterity;
		int ActivationSpeed;
		int ZoneInterferenceStrength;
		int CalculationCapacity;
	};
}