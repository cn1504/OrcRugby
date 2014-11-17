#include "CharacterStats.h"

namespace Core
{
	
	CharacterStats::CharacterStats(std::istream& in)
	{
		// Set Defaults
		Strength = 5;
		Dexterity = 5;
		ActivationSpeed = 5;
		ZoneInterferenceStrength = 5;
		CalculationCapacity = 5;

		Load(in);
	}

	CharacterStats::~CharacterStats() {}

	void CharacterStats::Load() {}
	
	void CharacterStats::Update() {}

	void CharacterStats::Load(std::istream& in)
	{
		std::string input;
		std::getline(in, input);
		Strength = std::stoi(input);
		std::getline(in, input);
		Dexterity = std::stoi(input);
		std::getline(in, input);
		ActivationSpeed = std::stoi(input);
		std::getline(in, input);
		ZoneInterferenceStrength = std::stoi(input);
		std::getline(in, input);
		CalculationCapacity = std::stoi(input);
	}

	void CharacterStats::Save(std::ostream& out)
	{
		out << Strength << std::endl;
		out << Dexterity << std::endl;
		out << ActivationSpeed << std::endl;
		out << ZoneInterferenceStrength << std::endl;
		out << CalculationCapacity << std::endl;
	}

}