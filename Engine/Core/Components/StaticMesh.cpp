#include "StaticMesh.h"
#include <Assets/AssetDB.h>

using namespace Core::Components;

StaticMesh::StaticMesh(std::string name, std::string material)
{
	Name = name;
	Material = material;
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::Draw(Core::Renderers::MeshRenderer* renderer)
{
	auto vao = Core::AssetDB->GetVertexArray(Name);
	auto material = Core::AssetDB->GetMaterial(Material);
	renderer->DrawMesh(*vao, *material, GetMatrix());
	
	Entity::Draw(renderer);
}