#include "TextureDebugPanel.h"
#include <Renderers/Shader.h>
#include <Assets/Texture.h>
#include <Assets/AssetDB.h>

using namespace Core::Components::Gui;


TextureDebugPanel::TextureDebugPanel() 
{ 
	Target = nullptr; 

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
TextureDebugPanel::~TextureDebugPanel() {}

void TextureDebugPanel::Draw(Core::Renderers::GuiRenderer* renderer)
{
	if (Target != nullptr)
	{
		renderer->DrawImage(*vb, *Target, GetMatrix());
	}

	Item::Draw(renderer);
}