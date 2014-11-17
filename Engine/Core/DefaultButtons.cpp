#include "DefaultButtons.h"

namespace Core
{
	namespace Gui
	{

		ExitButton::ExitButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}


		ExitButton::~ExitButton()
		{
		}


		void ExitButton::OnClick(int button)
		{
			WindowPtr->Close();
		}


		MaximizeButton::MaximizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* fsImage, Texture* fsMouseOverImage, Texture* fsMouseDownImage, Texture* winImage, Texture* winMouseOverImage, Texture* winMouseDownImage)
			: Item(parent, alignment, position, size, nullptr, winImage, winMouseOverImage, winMouseDownImage)
		{
			WindowPtr = window;

			FullscreenImage = fsImage;
			FullscreenMouseOverImage = fsMouseOverImage;
			FullscreenMouseDownImage = fsMouseDownImage;
			WindowedImage = winImage;
			WindowedMouseOverImage = winMouseOverImage;
			WindowedMouseDownImage = winMouseDownImage;

			UpdateImage();
		}




		MaximizeButton::~MaximizeButton()
		{
		}


		void MaximizeButton::OnClick(int button)
		{
			WindowPtr->Input->ToggleFullscreen();
			UpdateImage();
		}


		void MaximizeButton::Invalidate()
		{
			Item::Invalidate();
			UpdateImage();
		}


		void MaximizeButton::UpdateImage()
		{
			Image = (Settings::Window::Fullscreen) ? FullscreenImage : WindowedImage;
			MouseOverImage = (Settings::Window::Fullscreen) ? FullscreenMouseOverImage : WindowedMouseOverImage;
			MouseDownImage = (Settings::Window::Fullscreen) ? FullscreenMouseDownImage : WindowedMouseDownImage;
		}




		MinimizeButton::MinimizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}


		MinimizeButton::~MinimizeButton()
		{
		}


		void MinimizeButton::OnClick(int button)
		{
			WindowPtr->Minimize();
		}




		ResizeButton::ResizeButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}


		ResizeButton::~ResizeButton()
		{
		}


		void ResizeButton::OnDrag(glm::vec2 deltaPosition)
		{
			WindowPtr->Resize(deltaPosition);
		}




		MoveButton::MoveButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}


		MoveButton::~MoveButton()
		{
		}


		void MoveButton::Invalidate()
		{
			auto pdim = Parent->GetDimensions();
			Size.x = pdim.z * 0.75f;

			Item::Invalidate();
		}


		void MoveButton::OnDrag(glm::vec2 deltaPosition)
		{
			WindowPtr->Move(deltaPosition);
			OriginalMousePosition -= deltaPosition;
		}

	}
}