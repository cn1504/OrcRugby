#pragma once
#include <Game.h>
#include <Components/Entity.h>
#include "Grid.h"

namespace Game
{
	namespace Components
	{
		class HexIF : public Core::Components::Entity
		{
		public:
			virtual ~HexIF() {}

			virtual void Build() = 0;
			virtual void Expand() = 0;
			virtual std::shared_ptr<HexIF> FindClosestHex(const glm::vec3& position, float parentDistance) = 0;
			virtual std::string GetTag() = 0;
			virtual glm::ivec2 GetCoords() = 0;
			virtual int GetOrientation() = 0;
		};

		class NullHex : public HexIF
		{
		public:
			NullHex() {}
			virtual ~NullHex() {}

			virtual void Build() { Core::Debug->Log("Error: Building NullHex."); }
			virtual void Expand() { Core::Debug->Log("Error: Expanding NullHex."); }
			virtual std::shared_ptr<HexIF> FindClosestHex(const glm::vec3& position, float parentDistance) { return nullptr; }
			virtual std::string GetTag() { Core::Debug->Log("Error: NullHex GetTag."); return "NULL"; }
			virtual glm::ivec2 GetCoords() { Core::Debug->Log("Error: NullHex GetCoords."); return glm::ivec2(); }
			virtual int GetOrientation() { return 0; }
		};

		class Hex : public HexIF
		{			
		private:
			Grid* grid;

			glm::ivec2 Coords;
			std::string Tag;
			int Orientation;
			bool hasBeenBuilt;
			
			static std::unique_ptr<NullHex> NH;

		public:
			Hex(Grid* grid, glm::ivec2 Coords);
			virtual ~Hex();

			virtual void Build();
			virtual void Expand();
			virtual std::shared_ptr<HexIF> FindClosestHex(const glm::vec3& position, float parentDistance);
			virtual std::string GetTag() { return Tag; }
			virtual glm::ivec2 GetCoords() { return Coords; }
			virtual int GetOrientation() { return Orientation; }
		};
	}
}