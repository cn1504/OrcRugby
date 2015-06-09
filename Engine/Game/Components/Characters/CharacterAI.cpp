#include "CharacterAI.h"
#include "Character.h"
#include "Power.h"
#include <Components/Log.h>

using namespace Game::Components::Characters;

CharacterAI::CharacterAI(Character* c)
{
	C = c;
	Elapsed = (float)rand() / (float)RAND_MAX * 3.0f;
	ActionDuration = 3.0f;
}

void CharacterAI::Update()
{
	Elapsed += Core::Time->Delta;
	
	if (Elapsed > ActionDuration)
	{
		auto pa = C->Primary->GetAttack("Primary");
		auto t = PickTarget(pa->GetTarget(), pa->GetType());
		if (t != nullptr)
		{
			Game::Log->Post(C->Name + " attacks " + t->Name + " with " + C->Primary->GetName() + ".");
			C->Primary->Use(C, t);

			Elapsed = 0.0f;
		}
	}
}

Character* CharacterAI::PickTarget(std::string category, std::string type)
{
	if (type == "Damage" || type == "Debuff")
	{
		auto f = C->GetFaction();
		std::vector<std::shared_ptr<Character>> targets;
		for (auto& faction : Game::Factions)
		{
			if (faction.first != f)
			{
				for (auto& c : faction.second)
				{
					targets.push_back(c);
				}
			}
		}
		if (targets.size() == 0)
		{
			return nullptr;
		}
		auto target = targets[rand() % targets.size()];
		return target.get();
	}
	else if (type == "Heal" || type == "Buff")
	{
		auto f = C->GetFaction();
		std::vector<std::shared_ptr<Character>> targets;
		for (auto& c : Game::Factions[f])
		{
			targets.push_back(c);
		}
		if (targets.size() == 0)
		{
			return nullptr;
		}
		auto target = targets[rand() % targets.size()];
		return target.get();
	}
	return nullptr;
}