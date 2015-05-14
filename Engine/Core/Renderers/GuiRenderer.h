#pragma once
#include <Core.h>
#include "Shader.h"
#include "VertexBuffer.h"

namespace Core
{
	namespace Renderers
	{
		class GuiRenderer
		{
		private:
			std::unique_ptr<Shader> GuiTexture;
			std::unique_ptr<Shader> Font;

		public:
			GuiRenderer();
			~GuiRenderer();

			void Draw(Core::Components::Gui::Item* items);
			void DrawImage(const VertexBuffer& geometry, const Core::Assets::Texture& texture, const glm::mat3& transform);
			void DrawText(const VertexBuffer& geometry, const Core::Assets::Texture& texture, const glm::vec4& color, const glm::mat3& transform);
		};
	}
}