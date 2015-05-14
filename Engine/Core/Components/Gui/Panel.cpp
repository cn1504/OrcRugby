#include "Panel.h"
#include <Renderers/Shader.h>
#include <Assets/Texture.h>
#include <Assets/AssetDB.h>

using namespace Core::Components::Gui;


Panel::Panel() : Panel("") {}
Panel::Panel(std::string image)
{
	Image = image;

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f
	};

	vb = std::make_unique<Core::Renderers::VertexBuffer>(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
}
Panel::~Panel() {}

void Panel::Draw(Core::Renderers::GuiRenderer* renderer)
{
	if (Image != "")
	{
		renderer->DrawImage(*vb, *Core::AssetDB->GetTexture(Image), GetMatrix());
	}

	Item::Draw(renderer);
}