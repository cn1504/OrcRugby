#include "StaticMesh.h"
#include <Assets/AssetDB.h>

using namespace Core::Components;

StaticMesh::StaticMesh(std::string name, const glm::vec4& color, const glm::vec4& msr)
{
	Name = name;
	Color = color;
	MSR = msr;
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
	renderer->DrawMesh(*indices, *vertices, *uvs, *normals, Color, MSR, GetMatrix());
	
	Entity::Draw(renderer);
}