#include "CharacterSpawner.h"
#include <Components/Generator.h>
#include <Components/Log.h>

using namespace Game::Tests;


CharacterSpawner::CharacterSpawner()
{
}

CharacterSpawner::~CharacterSpawner()
{
}

void CharacterSpawner::Update()
{
	Entity::Update();
	
	if (Game::Factions["Enemy"].size() < 1)
	{
		auto c = Game::Generator->GenerateCharacter();
		c->SetFaction("Enemy");
		Core::Scene->AddChild(c);
		Game::Factions["Enemy"].push_back(c);
		Game::Log->Post("A wild " + c->GetName() + " has appeared!");
	}

	if (Game::Factions["Player"].size() < 1)
	{
		auto c = Game::Generator->GenerateCharacter();
		c->SetFaction("Player");
		Core::Scene->AddChild(c);
		Game::Factions["Player"].push_back(c);
		Game::Log->Post(c->GetName() + ", I choose you!");
	}
}
