#pragma once

#include <Game.h>
#include <Assets/Database.h>


namespace Game
{
	namespace Components
	{
		class SaveDB
		{
		private:
			std::shared_ptr<Core::Assets::Database> db;

		public:
			SaveDB(std::shared_ptr<Core::Assets::Database> db);
			virtual ~SaveDB();
		};
	}
}