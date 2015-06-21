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
			class Panel : public Item
			{
			protected:
				std::unique_ptr<Core::Renderers::VertexArray> vao;
				std::string Image;

			public:
				Panel(const Panel&) = delete;
				Panel& operator=(const Panel&) = delete;

				Panel();
				Panel(std::string image);
				virtual ~Panel();

				virtual void Draw(Core::Renderers::GuiRenderer* renderer);

				virtual void SetImage(std::string image) { Image = image; }
			};
		}
	}
}