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

			int GetInt(std::string key);
			double GetDouble(std::string key);
			float GetFloat(std::string key);
			std::string GetString(std::string key);
		};
	}
}