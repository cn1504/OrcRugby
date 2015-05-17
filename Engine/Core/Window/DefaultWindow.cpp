#include "DefaultWindow.h"
#include <Renderers/Renderers.h>
#include <Components/Gui/Panel.h>
#include <Input/Map.h>

using namespace Core::Window;


DefaultWindow::DefaultWindow(DefaultWindow && o)
	: Window(std::move(o))
{
	Top = std::move(o.Top);
	Gui = std::move(o.Gui);
	Camera = std::move(o.Camera);
	GeometryRenderer = std::move(o.GeometryRenderer);
	GuiRenderer = std::move(o.GuiRenderer);
}
DefaultWindow& DefaultWindow::operator=(DefaultWindow && o)
{
	Top = std::move(o.Top);
	Gui = std::move(o.Gui);
	Camera = std::move(o.Camera);
	GeometryRenderer = std::move(o.GeometryRenderer);
	GuiRenderer = std::move(o.GuiRenderer);
	Window::operator=(std::move(o));
	return *this;
}

DefaultWindow::DefaultWindow(std::string name, const glm::ivec2& position, const glm::ivec2& size)
	: DefaultWindow(name, position, size, std::make_unique<Windowed>(this))
{}

DefaultWindow::DefaultWindow(std::string name, const glm::ivec2& position, const glm::ivec2& size, std::unique_ptr<State> state)
	: Window(name, position, size, std::move(state))
{
	const float FOVY = 60.0f;
	const float MIN_DRAWDISTANCE = 0.1f;
	const float MAX_DRAWDISTANCE = 100.0f;

	Gui = std::make_shared<Core::Components::Gui::Panel>();
	Gui->Scale(glm::vec2((float)size.x, (float)size.y));
	Top = std::make_shared<Core::Components::Gui::Panel>();
	Top->Scale(glm::vec2(1.0f / ((float)size.x), 1.0f / ((float)size.y)));
	Top->AddChild(Gui);

	Camera = std::make_shared<Core::Components::Camera>();
	Camera->UpdateProjection(FOVY, ((float)size.x) / ((float)size.y), MIN_DRAWDISTANCE, MAX_DRAWDISTANCE);
	Core::Scene->AddChild(Camera);

	GeometryRenderer = std::make_unique<Core::Renderers::GeometryRenderer>();
	GeometryRenderer->SetCamera(Camera);
	LightRenderer = std::make_unique<Core::Renderers::LightRenderer>();
	LightRenderer->SetCamera(Camera);
	PostProcessingRenderer = std::make_unique<Core::Renderers::PostProcessingRenderer>();
	GuiRenderer = std::make_unique<Core::Renderers::GuiRenderer>();

	GeometryBuffer = std::make_unique<Core::Renderers::GBuffer>(size);
	LightBuffer = std::make_unique<Core::Renderers::LightBuffer>(size);

	FullscreenAction = std::make_shared<Core::Window::SwitchToFullscreen>(this);
	WindowedAction = std::make_shared<Core::Window::SwitchToWindowed>(this);
	IconifyAction = std::make_shared<Core::Window::Iconify>(this);
	PreviousMonitorAction = std::make_shared<Core::Window::MoveToPreviousMonitor>(this);
	NextMonitorAction = std::make_shared<Core::Window::MoveToNextMonitor>(this);
	InputMap->AddReleaseAction("Alt + Enter", std::weak_ptr<Core::Input::Action>(FullscreenAction));
	InputMap->AddReleaseAction("Alt + Up", std::weak_ptr<Core::Input::Action>(WindowedAction));
	InputMap->AddReleaseAction("Alt + Down", std::weak_ptr<Core::Input::Action>(IconifyAction));
	InputMap->AddReleaseAction("Alt + Left", std::weak_ptr<Core::Input::Action>(PreviousMonitorAction));
	InputMap->AddReleaseAction("Alt + Right", std::weak_ptr<Core::Input::Action>(NextMonitorAction));
}
DefaultWindow::~DefaultWindow() 
{
	Core::Scene->RemoveChild(Camera);
}

void DefaultWindow::Scale(const glm::ivec2& delta)
{
	Window::Scale(delta);
	
	if (Size.x != 0 && Size.y != 0)
	{
		auto rd = Size - GeometryBuffer->GetSize();

		const float FOVY = 60.0f;
		const float MIN_DRAWDISTANCE = 0.1f;
		const float MAX_DRAWDISTANCE = 100.0f;

		glm::vec2 shift = glm::vec2(1.0f + (float)rd.x / (float)(Gui->GetScale().x), 1.0f + (float)rd.y / (float)(Gui->GetScale().y));
		Gui->Scale(shift);
		Top->Scale(1.0f / shift);

		auto res = Gui->GetScale();

		Camera->UpdateProjection(FOVY, res.x / res.y, MIN_DRAWDISTANCE, MAX_DRAWDISTANCE);

		GeometryBuffer->Scale(rd);
		LightBuffer->Scale(rd);
	}
}

int DefaultWindow::Update()
{
	BeginUpdate();

	Top->Update();

	GeometryBuffer->SetAsTarget();
	GeometryBuffer->Clear();

	GeometryRenderer->DrawScene();

	LightBuffer->SetAsTarget();
	LightBuffer->Clear();
	
	LightRenderer->SetGeometryTextures(*GeometryBuffer->Depth, *GeometryBuffer->Normal, *GeometryBuffer->BaseColor, *GeometryBuffer->MSR);
	LightRenderer->DrawScene(glm::vec2(Size.x, Size.y));
	
	ScreenBuffer->SetAsTarget();
	ScreenBuffer->Clear();

	PostProcessingRenderer->SetTextures(*LightBuffer->Diffuse, *LightBuffer->Specular);
	PostProcessingRenderer->Draw(Size);

	if (Top != nullptr)
		GuiRenderer->Draw(Top.get());

	return Window::Update();
}



Minimized::Minimized(Window* context)
	: State(context)
{
	CloseAction = std::make_unique<Core::Window::Close>(Context);
	MinimizeAction = std::make_unique<DoNothing>(Context);
	MaximizeAction = std::make_unique<SwitchToFullscreen>(Context);
	RestoreAction = std::make_unique<SwitchToWindowed>(Context);
	MoveAction = std::make_unique<Core::Space::DontTranslate2DInt>(Context);
	ResizeAction = std::make_unique<Core::Space::DontScale2DInt>(Context);
	FocusAction = std::make_unique<DoNothing>(Context);
	DefocusAction = std::make_unique<DoNothing>(Context);
};
Minimized::Minimized(Minimized && o) : State(std::move(o)) {}
Minimized& Minimized::operator=(Minimized && o) { State::operator=(std::move(o)); return *this; }
Minimized::~Minimized() {}
std::string Minimized::GetName() { return "MinimizedDefaultWindow"; }


Windowed::Windowed(Window* context)
	: State(context)
{
	CloseAction = std::make_unique<Core::Window::Close>(Context);
	MinimizeAction = std::make_unique<Iconify>(Context);
	MaximizeAction = std::make_unique<SwitchToFullscreen>(Context);
	RestoreAction = std::make_unique<DoNothing>(Context);
	MoveAction = std::make_unique<Core::Space::Translate2DInt>(Context);
	ResizeAction = std::make_unique<Core::Space::Scale2DInt>(Context);
	FocusAction = std::make_unique<TakeFocus>(Context);
	DefocusAction = std::make_unique<DoNothing>(Context);
};
Windowed::Windowed(Windowed && o) : State(std::move(o)) {}
Windowed& Windowed::operator=(Windowed && o) { State::operator=(std::move(o)); return *this; }
Windowed::~Windowed() {}
std::string Windowed::GetName() { return "WindowedDefaultWindow"; }


Fullscreen::Fullscreen(Window* context)
	: State(context)
{
	CloseAction = std::make_unique<Core::Window::Close>(Context);
	MinimizeAction = std::make_unique<Iconify>(Context);
	MaximizeAction = std::make_unique<DoNothing>(Context);
	RestoreAction = std::make_unique<SwitchToWindowed>(Context);
	MoveAction = std::make_unique<Core::Space::DontTranslate2DInt>(Context);
	ResizeAction = std::make_unique<Core::Space::DontScale2DInt>(Context);
	FocusAction = std::make_unique<TakeFocus>(Context);
	DefocusAction = std::make_unique<DoNothing>(Context);
};
Fullscreen::Fullscreen(Fullscreen && o) : State(std::move(o)) {}
Fullscreen& Fullscreen::operator=(Fullscreen && o) { State::operator=(std::move(o)); return *this; }
Fullscreen::~Fullscreen() {}
std::string Fullscreen::GetName() { return "FullscreenDefaultWindow"; }
