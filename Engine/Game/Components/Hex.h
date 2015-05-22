#pragma once
#include <Game.h>
#include <Components/Entity.h>

namespace Game
{
	namespace Components
	{
		class Hex;

		class HexIF
		{
		public:
			virtual ~HexIF() {}

			virtual void Build() = 0;
			virtual void Expand() = 0;
			virtual void SetNeighbor(size_t i, HexIF* neighbor) = 0;
			virtual std::shared_ptr<Hex> FindClosestHex(const glm::vec3& position, float parentDistance) = 0;
			virtual std::string GetTag() = 0;
			virtual glm::ivec2 GetCoords() = 0;
		};

		class NullHex : public HexIF
		{
		public:
			NullHex() {}
			virtual ~NullHex() {}

			virtual void Build() { Core::Debug->Log("Error: Building NullHex."); }
			virtual void Expand() { Core::Debug->Log("Error: Expanding NullHex."); }
			virtual void SetNeighbor(size_t i, HexIF* neighbor) { Core::Debug->Log("Error: Setting NullHex Neighbor."); }
			virtual std::shared_ptr<Hex> FindClosestHex(const glm::vec3& position, float parentDistance) { return nullptr; }
			virtual std::string GetTag() { Core::Debug->Log("Error: NullHex GetTag."); return "NULL"; }
			virtual glm::ivec2 GetCoords() { Core::Debug->Log("Error: NullHex GetCoords."); return glm::ivec2(); }
		};

		class Hex : public Core::Components::Entity, public HexIF
		{			
		private:
			glm::ivec2 Coords;
			std::string Tag;
			int Orientation;
			bool hasBeenBuilt;

			std::vector<HexIF*> Neighbors;

			static std::unique_ptr<NullHex> NH;

		public:
			Hex(glm::ivec2 Coords);
			virtual ~Hex();

			virtual void Build();
			virtual void Expand();
			virtual void SetNeighbor(size_t i, HexIF* neighbor);
			virtual std::shared_ptr<Hex> FindClosestHex(const glm::vec3& position, float parentDistance);
			virtual std::string GetTag() { return Tag; }
			virtual glm::ivec2 GetCoords() { return Coords; }
		};
	}
}