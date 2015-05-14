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
		};

		class Hex : public Core::Components::Entity, public HexIF
		{			
		private:
			static int NextIndex;
			int Index;
			std::vector<HexIF*> Neighbors;

			static std::unique_ptr<NullHex> NH;

		public:
			Hex();
			virtual ~Hex();

			virtual void Build();
			virtual void Expand();
			virtual void SetNeighbor(size_t i, HexIF* neighbor);
			virtual std::shared_ptr<Hex> FindClosestHex(const glm::vec3& position, float parentDistance);
		};
	}
}