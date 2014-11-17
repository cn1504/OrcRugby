#pragma once
#include "Core.h"
#include "Window.h"
#include "GuiItem.h"

namespace Core
{
	namespace Gui
	{

		class ExitButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			ExitButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~ExitButton();

			virtual void OnClick(int button);
		};


		class MaximizeButton
			: public Item
		{
		protected:
			Window* WindowPtr;

			Texture* FullscreenImage;
			Texture* FullscreenMouseOverImage;
			Texture* FullscreenMouseDownImage;
			Texture* WindowedImage;
			Texture* WindowedMouseOverImage;
			Texture* WindowedMouseDownImage;

			void UpdateImage();

		public:
			MaximizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* fsImage = nullptr, Texture* fsMouseOverImage = nullptr, Texture* fsMouseDownImage = nullptr, Texture* winImage = nullptr, Texture* winMouseOverImage = nullptr, Texture* winMouseDownImage = nullptr);
			virtual ~MaximizeButton();

			virtual void OnClick(int button);
			virtual void Invalidate();
		};


		class MinimizeButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			MinimizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MinimizeButton();

			virtual void OnClick(int button);
		};


		class ResizeButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			ResizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~ResizeButton();

			virtual void OnDrag(glm::vec2 deltaPosition);
		};


		class MoveButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			MoveButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MoveButton();

			virtual void Invalidate();
			virtual void OnDrag(glm::vec2 deltaPosition);
		};

	}
}