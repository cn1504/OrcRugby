#pragma once
#include <Core.h>
#include <Space/Translatable.h>
#include <Space/Scalable.h>

namespace Core
{
	namespace Window
	{
		class State
		{
		protected:
			Window* Context;

			std::unique_ptr<WindowAction> CloseAction;
			std::unique_ptr<WindowAction> MinimizeAction;
			std::unique_ptr<WindowAction> MaximizeAction;
			std::unique_ptr<WindowAction> RestoreAction;
			std::unique_ptr<Core::Space::Translate2DIntAction> MoveAction;
			std::unique_ptr<Core::Space::Scale2DIntAction> ResizeAction;
			std::unique_ptr<WindowAction> FocusAction;
			std::unique_ptr<WindowAction> DefocusAction;

		public:
			State() = delete;
			State(const State&) = delete;
			State& operator=(const State&) = delete;
			State(State &&);
			State& operator=(State &&);

			State(Window* context);
			virtual ~State();
			virtual std::string GetName() = 0;

			virtual void Close();
			virtual void Minimize();
			virtual void Maximize();
			virtual void Restore();
			virtual void Move(const glm::ivec2& delta);
			virtual void Resize(const glm::ivec2& delta);
			virtual void Focus();
			virtual void Defocus();
		};
	}
}