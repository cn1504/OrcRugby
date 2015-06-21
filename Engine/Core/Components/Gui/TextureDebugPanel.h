#pragma once
#include "Item.h"
#include <Renderers/GuiRenderer.h>
#include <Renderers/VertexArray.h>

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			class TextureDebugPanel : public Item
			{
			protected:
				std::unique_ptr<Core::Renderers::VertexArray> vao;
				Core::Assets::Texture* Target;

			public:
				TextureDebugPanel(const TextureDebugPanel&) = delete;
				TextureDebugPanel& operator=(const TextureDebugPanel&) = delete;

				TextureDebugPanel();
				virtual ~TextureDebugPanel();

				virtual void Draw(Core::Renderers::GuiRenderer* renderer);

				virtual void SetTexture(Core::Assets::Texture* target) { Target = target; }
			};
		}
	}
}