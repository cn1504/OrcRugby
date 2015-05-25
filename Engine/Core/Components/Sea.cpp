#include "Sea.h"
#include <Assets/AssetDB.h>

using namespace Core::Components;

Sea::Sea(std::string name)
{
	Name = name;
}

Sea::~Sea()
{
}

void Sea::Draw(Core::Renderers::GeometryRenderer* renderer)
{
	auto indices = Core::AssetDB->GetVertexBuffer(Name + "_Indices");
	auto vertices = Core::AssetDB->GetVertexBuffer(Name + "_Vertices");
	auto uvs = Core::AssetDB->GetVertexBuffer(Name + "_Uvs");
	auto normals = Core::AssetDB->GetVertexBuffer(Name + "_Normals");
	renderer->DrawSea(*indices, *vertices, *uvs, *normals, GetMatrix());

	Entity::Draw(renderer);
}