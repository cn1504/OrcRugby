#include "Grid.h"
#include "Hex.h"

using namespace Game::Components;

/*
#include <Assets/AssetDB.h>
#include <Assets/Database.h>
const float SIDE_LENGTH = 25.0f;
const float SIDE_LENGTH_HALF = SIDE_LENGTH * 0.5f;
const float HEIGHT = glm::sqrt(SIDE_LENGTH*SIDE_LENGTH - SIDE_LENGTH_HALF*SIDE_LENGTH_HALF);
*/

Grid::Grid()
{
	auto center = std::make_shared<Hex>(glm::ivec2());
	AddChild(center);
	center->Build();

	/*
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> Uvs;
	std::vector<unsigned short> Indices;

	float height = glm::sqrt(SIDE_LENGTH*SIDE_LENGTH - SIDE_LENGTH_HALF*SIDE_LENGTH_HALF);
	Vertices.push_back(glm::vec3(0, 0, 0));
	Vertices.push_back(glm::vec3(SIDE_LENGTH, 0, 0));
	Vertices.push_back(glm::vec3(SIDE_LENGTH_HALF, 0, height));
	Vertices.push_back(glm::vec3(-SIDE_LENGTH_HALF, 0, height));
	Vertices.push_back(glm::vec3(-SIDE_LENGTH, 0, 0));
	Vertices.push_back(glm::vec3(-SIDE_LENGTH_HALF, 0, -height));
	Vertices.push_back(glm::vec3(SIDE_LENGTH_HALF, 0, -height));

	for (auto& v : Vertices)
	{
		Normals.push_back(glm::vec3(0, 1, 0));
		Uvs.push_back(glm::vec2(v.x, v.z) / SIDE_LENGTH);
	}

	Indices.push_back(0);
	Indices.push_back(2);
	Indices.push_back(1);
	Indices.push_back(0);
	Indices.push_back(3);
	Indices.push_back(2);
	Indices.push_back(0);
	Indices.push_back(4);
	Indices.push_back(3);
	Indices.push_back(0);
	Indices.push_back(5);
	Indices.push_back(4);
	Indices.push_back(0);
	Indices.push_back(6);
	Indices.push_back(5);
	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(6);

	unsigned char* data = new unsigned char[Vertices.size() * sizeof(glm::vec3)];
	memcpy(data, &Vertices[0], Vertices.size() * sizeof(glm::vec3));

	Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
	Core::Database->Bind(1, std::string("Hex_Vertices"));
	Core::Database->Bind(2, Core::Assets::Blob(data, Vertices.size() * sizeof(glm::vec3)));
	Core::Database->ExecuteAndFree();

	unsigned char* data2 = new unsigned char[Normals.size() * sizeof(glm::vec3)];
	memcpy(data2, &Normals[0], Normals.size() * sizeof(glm::vec3));

	Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
	Core::Database->Bind(1, std::string("Hex_Normals"));
	Core::Database->Bind(2, Core::Assets::Blob(data2, Normals.size() * sizeof(glm::vec3)));
	Core::Database->ExecuteAndFree();

	unsigned char* data3 = new unsigned char[Uvs.size() * sizeof(glm::vec2)];
	memcpy(data3, &Uvs[0], Uvs.size() * sizeof(glm::vec2));

	Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
	Core::Database->Bind(1, std::string("Hex_Uvs"));
	Core::Database->Bind(2, Core::Assets::Blob(data3, Uvs.size() * sizeof(glm::vec2)));
	Core::Database->ExecuteAndFree();

	unsigned char* data4 = new unsigned char[Indices.size() * sizeof(unsigned short)];
	memcpy(data4, &Indices[0], Indices.size() * sizeof(unsigned short));

	Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
	Core::Database->Bind(1, std::string("Hex_Indices"));
	Core::Database->Bind(2, Core::Assets::Blob(data4, Indices.size() * sizeof(unsigned short)));
	Core::Database->ExecuteAndFree(); */
}

Grid::~Grid() {}

std::shared_ptr<Hex> Grid::FindClosestHex(const glm::vec3& position)
{
	auto c = GetChild(0);
	auto chex = std::dynamic_pointer_cast<Hex>(c);
	return chex->FindClosestHex(position, FLT_MAX);
}
