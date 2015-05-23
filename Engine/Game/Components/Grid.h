#pragma once
#include <Game.h>
#include <Components/Entity.h>

namespace Game
{
	namespace Components
	{
		class HexIF;

		struct KeyHasher
		{
			std::size_t operator()(const glm::ivec2& k) const
			{
				using std::size_t;
				using std::hash;
				using std::string;

				return (hash<int>()(k.r)
					^ (hash<int>()(k.g) << 16));
			}
		};

		class Grid : public Core::Components::Entity
		{
		private:
			std::unordered_map<glm::ivec2, std::shared_ptr<HexIF>, KeyHasher> Map;

		public:
			Grid();
			virtual ~Grid();

			std::shared_ptr<HexIF> FindClosestHex(const glm::vec3& position);
			std::shared_ptr<HexIF> GetHex(glm::ivec2 coords);
		};
	}
}