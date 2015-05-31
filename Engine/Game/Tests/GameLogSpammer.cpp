#include "GameLogSpammer.h"
#include <Components/Log.h>

using namespace Game::Tests;


GameLogSpammer::GameLogSpammer()
{
	line = 0;
	timeElapsed = 0.0f;
	waitDuration = 4.0f;

	testLines.push_back("A level 1 Stubborn Troll appears in the long grass.");
	testLines.push_back("Stubborn Troll uses faulty generalization: \"If you're poor, stop being poor.\"");
	testLines.push_back("Social Justice Paladin deftly outmaneuvers the troll's position.");
	testLines.push_back("Stubborn Troll attempts to brute force through your reason:");
	testLines.push_back("\"I built my life one success at a time.  Poor people should try harder to better themselves too.\"");
	testLines.push_back("Social Justice Paladin attempts to counter...");
	testLines.push_back("\"You're assuming they're not already trying.\" - [FAILURE] - Loses 5 sanity.");
}
GameLogSpammer::~GameLogSpammer()
{
}

void GameLogSpammer::Update()
{
	Entity::Update();

	timeElapsed += Core::Time->Delta;
	while (timeElapsed > waitDuration)
	{
		Game::Log->Post(testLines[line]);
		line = (line + 1) % testLines.size();

		timeElapsed -= waitDuration;
	}
}