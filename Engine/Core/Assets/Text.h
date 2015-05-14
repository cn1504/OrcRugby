#pragma once

#include <Core.h>
#include <Renderers/Shader.h>
#include <Window/Window.h>
#include "Font.h"

namespace Core
{
	namespace Assets
	{

		class Text
		{
		private:
			std::string Txt;
			int newLines;
			Font* Font;
			glm::vec2 Offset;
			glm::vec2 Position;
			glm::vec4 Color;
			GLuint VertexBuffer;

		public:
			Text(std::string txt, int x, int y, Core::Assets::Font* font, glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0));
			~Text();

			void SetColor(glm::vec4 color);
			void Move(glm::vec2 position);
			std::string GetText();

			void UpdateText(std::string txt);
			void BuildGeometry(const glm::vec2& size);

			void Render(Core::Renderers::Shader* shader);
		};

	}
}