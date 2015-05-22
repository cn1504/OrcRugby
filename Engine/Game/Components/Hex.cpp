#include "Hex.h"
#include <Components/StaticMesh.h>
#include <Components/LightSource.h>
#include <Assets/AssetDB.h>
#include <Components/SaveDB.h>

using namespace Game::Components;

const float SIDE_LENGTH = 25.0f;
const float SIDE_LENGTH_HALF = SIDE_LENGTH * 0.5f;
const float HEIGHT = glm::sqrt(SIDE_LENGTH*SIDE_LENGTH - SIDE_LENGTH_HALF*SIDE_LENGTH_HALF);

std::unique_ptr<NullHex> Hex::NH = std::make_unique<NullHex>();

Hex::Hex(glm::ivec2 Coords) : Coords(Coords)
{
	Tag = "NULL";
	hasBeenBuilt = false;
	Orientation = 0;

	Neighbors.resize(6, NH.get());
}

Hex::~Hex()
{
}

void Hex::SetNeighbor(size_t i, HexIF* neighbor)
{
	Neighbors[i] = neighbor;
}

void Hex::Build()
{
	if (hasBeenBuilt)
		return;

	Core::Debug->Log("Building Tile: " + std::to_string(Coords));
	Tag = Game::Save->GetTileTag(Coords);

	if (Tag == "")
	{
		Tag = "Grassland_01";
		Orientation = 0;
		Game::Save->SetTile(Coords, Tag, Orientation);
	}
	else
	{
		Orientation = Game::Save->GetTileOrientation(Coords);
	}

	for (int i = 0; i < Orientation; i++)
		Rotate(glm::vec3(0, glm::radians(60.0), 0));

	Core::AssetDB->AddTileContents(shared_from_this(), Tag);
	
	hasBeenBuilt = true;
}

void Hex::Expand()
{
	std::vector<size_t> toBuild;

	for (size_t i = 0; i < 6; i++)
	{
		if (Neighbors[i] == NH.get())
		{
			std::shared_ptr<Hex> hex;
			auto myPos = glm::vec3(GetMatrix() * glm::vec4(0, 0, 0, 1));

			if (i == 0)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x, Coords.y + 1));
				hex->Translate(myPos + glm::vec3(0, 0, 2.0f * HEIGHT));
			}
			else if (i == 1)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x + 1, Coords.y + ((Coords.x % 2 == 1) ? 0 : 1)));
				hex->Translate(myPos + glm::vec3(-(SIDE_LENGTH + SIDE_LENGTH_HALF), 0, HEIGHT));
			}
			else if (i == 2)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x + 1, Coords.y + ((Coords.x % 2 == 1) ? -1 : 0)));
				hex->Translate(myPos + glm::vec3(-(SIDE_LENGTH + SIDE_LENGTH_HALF), 0, -HEIGHT));
			}
			else if (i == 3)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x, Coords.y - 1));
				hex->Translate(myPos + glm::vec3(0, 0, -2.0f * HEIGHT));
			}
			else if (i == 4)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x - 1, Coords.y + ((Coords.x % 2 == 1) ? -1 : 0)));
				hex->Translate(myPos + glm::vec3(SIDE_LENGTH + SIDE_LENGTH_HALF, 0, -HEIGHT));
			}
			else if (i == 5)
			{
				hex = std::make_shared<Hex>(glm::ivec2(Coords.x - 1, Coords.y + ((Coords.x % 2 == 1) ? 0 : 1)));
				hex->Translate(myPos + glm::vec3(SIDE_LENGTH + SIDE_LENGTH_HALF, 0, HEIGHT));
			}

			Neighbors[i] = hex.get();
			toBuild.push_back(i);
			GetParent()->AddChild(hex);
		}
	}

	for (size_t i = 0; i < 6; i++)
	{
		Neighbors[i]->SetNeighbor((i + 2) % 6, Neighbors[(i + 1) % 6]);
		Neighbors[i]->SetNeighbor((i + 3) % 6, this);
		Neighbors[i]->SetNeighbor((i + 4) % 6, Neighbors[(i + 5) % 6]);
	}

	for (auto& i : toBuild)
	{
		Neighbors[i]->Build();
	}
}

std::shared_ptr<Hex> Hex::FindClosestHex(const glm::vec3& position, float parentDistance)
{
	float myDistance = glm::distance(position, glm::vec3(GetMatrix() * glm::vec4(0, 0, 0, 1)));
	if (myDistance >= parentDistance)
	{
		return nullptr;
	}

	for (size_t i = 0; i < 6; i++)
	{
		auto ch = Neighbors[i]->FindClosestHex(position, myDistance);
		if (ch != nullptr)
		{
			return ch;
		}
	}
	
	return std::dynamic_pointer_cast<Hex>(shared_from_this());
}