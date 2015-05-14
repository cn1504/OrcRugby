#pragma once
#include "Item.h"
#include <Renderers/GuiRenderer.h>
#include <Renderers/VertexBuffer.h>

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			class Label : public Item
			{
			private:
				std::string Font;
				glm::vec4 Color;
				std::string Text;
				int Alignment;

				std::unique_ptr<Core::Renderers::VertexBuffer> vb;

			public:
				Label(std::string font, const glm::vec4& color, std::string text);
				Label(std::string font, const glm::vec4& color, int alignment, std::string text);
				virtual ~Label();

				virtual void Draw(Core::Renderers::GuiRenderer* renderer);

				virtual void SetText(std::string text);
			};
		}
	}
}