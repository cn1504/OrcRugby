#include "Sea.h"
#include <Assets/AssetDB.h>
#include <Renderers/VertexArray.h>

using namespace Core::Components;

Sea::Sea(std::string name)
{
	Name = name;
}

Sea::~Sea()
{
}

void Sea::Draw(Core::Renderers::MeshRenderer* renderer)
{
	auto vao = Core::AssetDB->GetVertexArray(Name);
	renderer->DrawSea(*vao, GetMatrix());

	Entity::Draw(renderer);
}