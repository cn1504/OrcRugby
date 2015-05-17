#pragma once

#include <Game.h>
#include <Assets/Database.h>

namespace Game
{
	namespace Components
	{
		class Generator
		{
		private:
			std::shared_ptr<Core::Assets::Database> db;

		public:
			Generator(std::shared_ptr<Core::Assets::Database> db) : db(db) {}

			std::string PickName(std::string race, std::string gender);
		};
	}
}