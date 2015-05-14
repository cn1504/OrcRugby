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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	//if (Settings::Video::ShowFPS)
	//{
	//	fpsText->UpdateText("TS: " + std::to_string(Time::Scale) + "\nDPF: " + std::to_string(Time::DrawCallsPerFrame) + "\nFPS : " + std::to_string((int)Time::FPS));
	//}

	items->Draw(this);
	
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	Debug->GLError("ERROR: failed to render ui.");
}


void GuiRenderer::DrawImage(const VertexBuffer& geometry, const Core::Assets::Texture& texture, const glm::mat3& transform)
{
	GuiTexture->Activate();

	GuiTexture->SetTexture("GuiTexture", texture, 0);

	// Draw quad
	GuiTexture->SetUniform("Transform", transform);
	GuiTexture->SetVec4Attribute("coord", geometry.GetID());
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)geometry.GetSize() / sizeof(glm::vec4));
	GuiTexture->DisableAttribute("coord");

	Debug->GLError("ERROR: Failed to render gui Panel.");
}

void GuiRenderer::DrawText(const VertexBuffer& geometry, const Core::Assets::Texture& texture, const glm::vec4& color, const glm::mat3& transform)
{
	Font->Activate();
	
	Font->SetTexture("tex", texture, 0);
	Font->SetUniform("color", color);

	// Draw quad
	Font->SetUniform("Transform", transform);
	Font->SetVec4Attribute("coord", geometry.GetID());
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)geometry.GetSize() / sizeof(glm::vec4));
	Font->DisableAttribute("coord");

	Debug->GLError("ERROR: Failed to render label.");
}
