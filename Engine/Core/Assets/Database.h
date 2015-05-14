#pragma once
#include <Core.h>

namespace Core {
	namespace Assets
	{
		class Blob
		{
		private:
			unsigned char* data;
			size_t size;

		public:
			Blob() = delete;
			Blob(const Blob&) = delete;
			Blob& operator=(const Blob&) = delete;

			Blob(Blob&& o)
			{
				std::swap(size, o.size);
				std::swap(data, o.data);
			}
			Blob& operator=(Blob&& o)
			{
				std::swap(size, o.size); 
				std::swap(data, o.data); 
				return *this;
			}

			Blob(unsigned char* data, size_t size) : data(data), size(size)	{}
			Blob(std::vector<unsigned char>& input)
			{
				size = input.size();
				data = new unsigned char[size];
				memcpy(data, &input[0], size);
			}
			Blob(size_t size) : size(size) { data = new unsigned char[size]; }

			~Blob() {
				if(size != 0)
					delete[] data;
			}

			unsigned char* GetData() const { return data; }
			size_t GetSize() const { return size; }
		};

		class Database
		{
		public:
			virtual ~Database() {}

			virtual void Sql(std::string query) = 0;
			virtual void SqlStatement(std::string query) = 0;
			virtual void FreeQuery() = 0;
			virtual void ExecuteAndFree() = 0;

			virtual bool FetchRow() = 0;
			virtual bool GetColumnBool(int column) = 0;
			virtual int GetColumnInt(int column) = 0;
			virtual double GetColumnDouble(int column) = 0;
			virtual std::string GetColumnString(int column) = 0;
			virtual std::unique_ptr<Blob> GetColumnBlob(int column) = 0;

			virtual void Bind(int position, bool value) = 0;
			virtual void Bind(int position, int value) = 0;
			virtual void Bind(int position, double value) = 0;
			virtual void Bind(int position, const std::string& value) = 0;
			virtual void Bind(int position, const Blob& value) = 0;
		};
	}
}