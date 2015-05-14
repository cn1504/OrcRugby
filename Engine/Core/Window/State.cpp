#include "State.h"
#include "WindowAction.h"

using namespace Core::Window;

State::State(State && o)
{
	Context = o.Context;

	CloseAction = std::move(CloseAction);
	MinimizeAction = std::move(MinimizeAction);
	MaximizeAction = std::move(MaximizeAction);
	RestoreAction = std::move(RestoreAction);
	MoveAction = std::move(MoveAction);
	ResizeAction = std::move(ResizeAction);
	FocusAction = std::move(FocusAction);
	DefocusAction = std::move(DefocusAction);
}

State& State::operator=(State && o)
{
	Context = o.Context;

	CloseAction = std::move(CloseAction);
	MinimizeAction = std::move(MinimizeAction);
	MaximizeAction = std::move(MaximizeAction);
	RestoreAction = std::move(RestoreAction);
	MoveAction = std::move(MoveAction);
	ResizeAction = std::move(ResizeAction);
	FocusAction = std::move(FocusAction);
	DefocusAction = std::move(DefocusAction);
}

State::State(Window* context)
{
	Context = context;

	CloseAction = std::make_unique<DoNothing>(Context);
	MinimizeAction = std::make_unique<DoNothing>(Context);
	MaximizeAction = std::make_unique<DoNothing>(Context);
	RestoreAction = std::make_unique<DoNothing>(Context);
	MoveAction = std::make_unique<Core::Space::DontTranslate2DInt>((Core::Space::Translatable2DInt*)Context);
	ResizeAction = std::make_unique<Core::Space::DontScale2DInt>((Core::Space::Scalable2DInt*)Context);
	FocusAction = std::make_unique<DoNothing>(Context);
	DefocusAction = std::make_unique<DoNothing>(Context);
}

State::~State() {}

void State::Close() { CloseAction->Perform(); }
void State::Minimize() { MinimizeAction->Perform(); }
void State::Maximize() { MaximizeAction->Perform(); }
void State::Restore() { RestoreAction->Perform(); }
void State::Move(const glm::ivec2& delta) { MoveAction->Perform(delta); }
void State::Resize(const glm::ivec2& delta) { ResizeAction->Perform(delta); }
void State::Focus() { FocusAction->Perform(); }
void State::Defocus() { DefocusAction->Perform(); }
