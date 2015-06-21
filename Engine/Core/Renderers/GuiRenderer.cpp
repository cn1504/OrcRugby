#include "GuiRenderer.h"
#include <Components/Gui/Item.h>

using namespace Core::Renderers;

GuiRenderer::GuiRenderer()
{
	GuiTexture = std::make_unique<Shader>("guitexture.vert", "guitexture.frag");
	Font = std::make_unique<Shader>("font.vert", "font.frag");
}

GuiRenderer::~GuiRenderer() {}

void GuiRenderer::Draw(Core::Components::Gui::Item* items)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	items->Draw(this);
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Debug->GLError("ERROR: failed to render ui.");
}


void GuiRenderer::DrawImage(const VertexArray& geometry, const Core::Assets::Texture& texture, const glm::mat3& transform)
{
	GuiTexture->Activate();

	GuiTexture->SetTexture("GuiTexture", texture, 0);

	// Draw quad
	GuiTexture->SetUniform("Transform", transform);
	geometry.Bind();
	glDrawArrays(GL_TRIANGLES, 0, geometry.GetSize());
	geometry.Unbind();

	Debug->GLError("ERROR: Failed to render gui Panel.");
}

void GuiRenderer::DrawText(const VertexArray& geometry, const Core::Assets::Texture& texture, const glm::vec4& color, const glm::mat3& transform)
{
	Font->Activate();
	
	Font->SetTexture("tex", texture, 0);
	Font->SetUniform("color", color);

	// Draw quad
	Font->SetUniform("Transform", transform);

	geometry.Bind();
	glDrawArrays(GL_TRIANGLES, 0, geometry.GetSize());
	geometry.Unbind();

	Debug->GLError("ERROR: Failed to render label.");
}
