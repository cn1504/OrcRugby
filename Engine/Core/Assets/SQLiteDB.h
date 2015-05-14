#pragma once
#include "Database.h"

namespace Core {
	namespace Assets
	{
		class SQLiteDB : public Database
		{
		private:
			sqlite3* db = nullptr;
			sqlite3_stmt* stmt = nullptr;
			std::string Query;

		public:
			SQLiteDB() = delete;
			SQLiteDB(const SQLiteDB&) = delete;
			SQLiteDB& operator=(const SQLiteDB&) = delete;

			SQLiteDB(std::string filepath);
			virtual ~SQLiteDB();
			
			void Sql(std::string query);
			void SqlStatement(std::string query);
			void FreeQuery();
			void ExecuteAndFree();

			bool FetchRow();
			bool GetColumnBool(int column);
			int GetColumnInt(int column);
			double GetColumnDouble(int column);
			std::string GetColumnString(int column);
			std::unique_ptr<Blob> GetColumnBlob(int column);

			void Bind(int position, bool value);
			void Bind(int position, int value);
			void Bind(int position, double value);
			void Bind(int position, const std::string& value);
			void Bind(int position, const Blob& value);
		};
	}
}