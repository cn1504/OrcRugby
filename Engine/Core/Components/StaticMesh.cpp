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

void StaticMesh::Draw(Core::Renderers::GeometryRenderer* renderer)
{
	auto indices = Core::AssetDB->GetVertexBuffer(Name + "_Indices");
	auto vertices = Core::AssetDB->GetVertexBuffer(Name + "_Vertices");
	auto uvs = Core::AssetDB->GetVertexBuffer(Name + "_Uvs");
	auto normals = Core::AssetDB->GetVertexBuffer(Name + "_Normals");
	auto material = Core::AssetDB->GetMaterial(Material);
	renderer->DrawMesh(*indices, *vertices, *uvs, *normals, *material, GetMatrix());
	
	Entity::Draw(renderer);
}