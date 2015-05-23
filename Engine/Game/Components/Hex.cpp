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

Hex::Hex(Grid* grid, glm::ivec2 Coords) : grid(grid), Coords(Coords)
{
	Tag = "NULL";
	hasBeenBuilt = false;
	Orientation = 0;
}

Hex::~Hex()
{
}

void Hex::Build()
{
	if (hasBeenBuilt)
		return;

	Tag = Game::Save->GetTileTag(Coords);
	Core::Debug->Log("Building Tile: " + std::to_string(Coords));

	if (Tag == "")
	{				
		// Build list of valid tiles that can be placed
		std::vector<std::pair<std::string, int>> matches;
		{
			// Determine edge requirements
			int reqs[6];

			for (int i = 0; i < 6; i++)
			{
				glm::ivec2 off;
				if (i == 0)
					off = glm::ivec2(0, 1);
				else if (i == 1)
					off = glm::ivec2(1, (abs(Coords.x) % 2 == 1) ? 0 : 1);
				else if (i == 2)
					off = glm::ivec2(1, (abs(Coords.x) % 2 == 1) ? -1 : 0);
				else if (i == 3)
					off = glm::ivec2(0, -1);
				else if (i == 4)
					off = glm::ivec2(-1, (abs(Coords.x) % 2 == 1) ? -1 : 0);
				else if (i == 5)
					off = glm::ivec2(-1, (abs(Coords.x) % 2 == 1) ? 0 : 1);
				auto n = grid->GetHex(Coords + off);

				reqs[i] = Core::AssetDB->GetTileEdgeType(n->GetTag(), (i + 3 + n->GetOrientation()) % 6);
			}
			Core::Debug->Log("\tTile Reqs: {" + std::to_string(reqs[0]) + ", " + std::to_string(reqs[1]) + ", " + std::to_string(reqs[2]) + ", " + std::to_string(reqs[3]) + ", " + std::to_string(reqs[4]) + ", " + std::to_string(reqs[5]) + "}");

			bool hasCoast = false;
			bool hasOcean = false;
			bool hasGrass = false;
			for (int i = 0; i < 6; i++)
			{
				if (reqs[i] == 1)
					hasGrass = true;
				if (reqs[i] == 2)
					hasCoast = true;
				if (reqs[i] == 3)
					hasOcean = true;
			}
			if (hasGrass && hasOcean)
				hasCoast = true;


			Core::Database->Sql("SELECT Tag, Edge_N, Edge_NE, Edge_SE, Edge_S, Edge_SW, Edge_NW FROM Tiles WHERE Spawnable=1");
			while (Core::Database->FetchRow())
			{
				int edges[6] = { Core::Database->GetColumnInt(1),
					Core::Database->GetColumnInt(2),
					Core::Database->GetColumnInt(3),
					Core::Database->GetColumnInt(4),
					Core::Database->GetColumnInt(5),
					Core::Database->GetColumnInt(6) };

				for (int o = 0; o < 6; o++)
				{
					bool match = true;
					if (hasCoast == false)
						for (int i = 0; i < 6; i++)
						{						
							if (edges[i] == 2)
							{
								match = false;
								break;
							}
						}
					if (match)
						for (int i = 0; i < 6; i++)
						{
							if (reqs[i] != 0)
							{
								auto edge = edges[(o + i) % 6];
								if (edge != reqs[i])
								{
									match = false;
									break;
								}
							}
						}
					if (match)
						matches.push_back(std::pair<std::string, int>(Core::Database->GetColumnString(0), o));
				}
			}
			Core::Database->FreeQuery();
		}

		// Choose tile from the list
		if (matches.size() == 0)
		{
			Core::Debug->Log("No valid tile found for " + std::to_string(Coords));
		}

		Core::Debug->Log("Match Count: " + std::to_string(matches.size()));
		int pick = std::rand() % matches.size();
		Tag = matches[pick].first;
		Orientation = matches[pick].second;

		// Save tile selection
		Game::Save->SetTile(Coords, Tag, Orientation);
	}
	else
	{
		Orientation = Game::Save->GetTileOrientation(Coords);		
	}

	for (int i = 0; i < Orientation; i++)
		Rotate(glm::vec3(0, glm::radians(60.0), 0));

	Core::Debug->Log("\t" + Tag + ", " + std::to_string(Orientation) + "; Position: " + std::to_string(GetPosition()));

	Core::AssetDB->AddTileContents(shared_from_this(), Tag);
	
	hasBeenBuilt = true;
}

void Hex::Expand()
{
	std::vector<size_t> toBuild;

	std::shared_ptr<Game::Components::HexIF> neighbors[6];
	neighbors[0] = grid->GetHex(Coords + glm::ivec2(0, 1));
	neighbors[3] = grid->GetHex(Coords + glm::ivec2(0, -1));
	if (abs(Coords.x) % 2 == 1)
	{
		neighbors[1] = grid->GetHex(Coords + glm::ivec2(1, 0));
		neighbors[2] = grid->GetHex(Coords + glm::ivec2(1, -1));
		neighbors[4] = grid->GetHex(Coords + glm::ivec2(-1, -1));
		neighbors[5] = grid->GetHex(Coords + glm::ivec2(-1, 0));
	}
	else
	{
		neighbors[1] = grid->GetHex(Coords + glm::ivec2(1, 1));
		neighbors[2] = grid->GetHex(Coords + glm::ivec2(1, 0));
		neighbors[4] = grid->GetHex(Coords + glm::ivec2(-1, 0));
		neighbors[5] = grid->GetHex(Coords + glm::ivec2(-1, 1));
	}

	for (int i = 0; i <= 6; i++)
	{
		if (i == 6)
		{
			for (int j = 0; j < 6; j++)
			{
				neighbors[j]->Build();
			}
			break;
		}

		if (neighbors[i]->GetTag() != "NULL")
		{
			for (int j = 1; j < 6; j++)
			{
				neighbors[(i - j + 6) % 6]->Build();
			}
			break;
		}
	}
}

std::shared_ptr<HexIF> Hex::FindClosestHex(const glm::vec3& position, float parentDistance)
{
	float myDistance = glm::distance(position, glm::vec3(GetMatrix() * glm::vec4(0, 0, 0, 1)));
	if (myDistance >= parentDistance)
	{
		return nullptr;
	}

	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<HexIF> n;
		
		if (i == 0)
			n = grid->GetHex(Coords + glm::ivec2(0, 1));
		else if (i == 3)
			n = grid->GetHex(Coords + glm::ivec2(0, -1));
		else if (i == 1)
			n = (Coords.x % 2 == 1) ? grid->GetHex(Coords + glm::ivec2(1, 0)) : grid->GetHex(Coords + glm::ivec2(1, 1));
		else if (i == 2)
			n = (Coords.x % 2 == 1) ? grid->GetHex(Coords + glm::ivec2(1, -1)) : grid->GetHex(Coords + glm::ivec2(1, 0));		
		else if (i == 4)
			n = (Coords.x % 2 == 1) ? grid->GetHex(Coords + glm::ivec2(-1, -1)) : grid->GetHex(Coords + glm::ivec2(-1, 0)); 
		else if (i == 5)
			n = (Coords.x % 2 == 1) ? grid->GetHex(Coords + glm::ivec2(-1, 0)) : grid->GetHex(Coords + glm::ivec2(-1, 1));

		auto ch = n->FindClosestHex(position, myDistance);
		if (ch != nullptr)
		{
			return ch;
		}
	}
	
	return std::dynamic_pointer_cast<HexIF>(shared_from_this());
}