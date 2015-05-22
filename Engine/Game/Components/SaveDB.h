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

			void Set(std::string key, int value);
			void Set(std::string key, double value);
			void Set(std::string key, float value);
			void Set(std::string key, std::string value);

			int GetInt(std::string key);
			double GetDouble(std::string key);
			float GetFloat(std::string key);
			std::string GetString(std::string key);

			void NewGame();

			std::string GetTileTag(const glm::ivec2& position);
			int GetTileOrientation(const glm::ivec2& position);
			void SetTile(const glm::ivec2& position, std::string tag, int orientation);
		};
	}
}