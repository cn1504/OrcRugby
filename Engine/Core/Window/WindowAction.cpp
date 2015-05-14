#include "WindowAction.h"
#include "Window.h"
#include "DefaultWindow.h"

using namespace Core::Window;


WindowAction::WindowAction(WindowAction && o) {	Target = o.Target; }
WindowAction& WindowAction::operator=(WindowAction && o) { Target = o.Target; }
WindowAction::WindowAction(Window* target) { Target = target; }
WindowAction::~WindowAction() {}


DoNothing::DoNothing(DoNothing && o) : WindowAction(std::move(o)) {}
DoNothing& DoNothing::operator=(DoNothing && o) { WindowAction::operator=(std::move(o)); return *this; }
DoNothing::DoNothing(Window* target) : WindowAction(target) {}
DoNothing::~DoNothing() {}
void DoNothing::Perform() {}


SwitchToFullscreen::SwitchToFullscreen(SwitchToFullscreen && o) : WindowAction(std::move(o)) {}
SwitchToFullscreen& SwitchToFullscreen::operator=(SwitchToFullscreen && o) { WindowAction::operator=(std::move(o)); return *this; }
SwitchToFullscreen::SwitchToFullscreen(Window* target) : WindowAction(target) {}
SwitchToFullscreen::~SwitchToFullscreen() {}
void SwitchToFullscreen::Perform()
{
	auto currentPosition = Target->GetPosition();
	int count, x, y;
	auto monitors = glfwGetMonitors(&count);
	auto vmode = glfwGetVideoMode(monitors[0]);
	for (int i = 0; i < count; i++)
	{		
		glfwGetMonitorPos(monitors[i], &x, &y);
		vmode = glfwGetVideoMode(monitors[i]);
		if (currentPosition.x > x && currentPosition.x < x + vmode->width)
			if (currentPosition.y > y && currentPosition.y < y + vmode->height)
			{
				break;
			}
	}

	Target->SetState(std::make_unique<Fullscreen>(Target));
	glfwSetWindowPos(Target->GetGLFWWindow(), x, y); 
	glfwSetWindowSize(Target->GetGLFWWindow(), vmode->width, vmode->height);
}


SwitchToWindowed::SwitchToWindowed(SwitchToWindowed && o) : WindowAction(std::move(o)) {}
SwitchToWindowed& SwitchToWindowed::operator=(SwitchToWindowed && o) { WindowAction::operator=(std::move(o)); return *this; }
SwitchToWindowed::SwitchToWindowed(Window* target) : WindowAction(target) {}
SwitchToWindowed::~SwitchToWindowed() {}
void SwitchToWindowed::Perform()
{
	if (glfwGetWindowAttrib(Target->GetGLFWWindow(), GLFW_ICONIFIED))
	{
		glfwRestoreWindow(Target->GetGLFWWindow());
		return;
	}

	auto currentPosition = Target->GetPosition();
	int count, x, y;
	auto monitors = glfwGetMonitors(&count);
	auto vmode = glfwGetVideoMode(monitors[0]);
	for (int i = 0; i < count; i++)
	{
		glfwGetMonitorPos(monitors[i], &x, &y);
		vmode = glfwGetVideoMode(monitors[i]);
		if (currentPosition.x >= x && currentPosition.x < x + vmode->width)
			if (currentPosition.y >= y && currentPosition.y < y + vmode->height)
			{
				break;
			}
	}

	Target->SetState(std::make_unique<Windowed>(Target));
	glfwSetWindowPos(Target->GetGLFWWindow(), x + vmode->width / 6, y + vmode->height / 6);
	glfwSetWindowSize(Target->GetGLFWWindow(), vmode->width * 2 / 3, vmode->height * 2 / 3);
}


Iconify::Iconify(Iconify && o) : WindowAction(std::move(o)) {}
Iconify& Iconify::operator=(Iconify && o) { WindowAction::operator=(std::move(o)); return *this; }
Iconify::Iconify(Window* target) : WindowAction(target) {}
Iconify::~Iconify() {}
void Iconify::Perform()
{
	Target->SetState(std::make_unique<Minimized>(Target));
	glfwIconifyWindow(Target->GetGLFWWindow());
}


TakeFocus::TakeFocus(TakeFocus && o) : WindowAction(std::move(o)) {}
TakeFocus& TakeFocus::operator=(TakeFocus && o) { WindowAction::operator=(std::move(o)); return *this; }
TakeFocus::TakeFocus(Window* target) : WindowAction(target) {}
TakeFocus::~TakeFocus() {}
void TakeFocus::Perform()
{
}


Close::Close(Close && o) : WindowAction(std::move(o)) {}
Close& Close::operator=(Close && o) { WindowAction::operator=(std::move(o)); return *this; }
Close::Close(Window* target) : WindowAction(target) {}
Close::~Close() {}
void Close::Perform()
{
	glfwSetWindowShouldClose(Target->GetGLFWWindow(), GL_TRUE);
}


MoveToPreviousMonitor::MoveToPreviousMonitor(MoveToPreviousMonitor && o) : WindowAction(std::move(o)) {}
MoveToPreviousMonitor& MoveToPreviousMonitor::operator=(MoveToPreviousMonitor && o) { WindowAction::operator=(std::move(o)); return *this; }
MoveToPreviousMonitor::MoveToPreviousMonitor(Window* target) : WindowAction(target) {}
MoveToPreviousMonitor::~MoveToPreviousMonitor() {}
void MoveToPreviousMonitor::Perform()
{
	int count, x, y;
	auto monitors = glfwGetMonitors(&count);
	if (count > 1)
	{
		auto currentPosition = Target->GetPosition();
		auto vmode = glfwGetVideoMode(monitors[0]);
		glfwGetMonitorPos(monitors[0], &x, &y);

		int i = 0;
		int relX = currentPosition.x - x;
		int relY = currentPosition.y - y;
		for (; i < count; i++)
		{
			glfwGetMonitorPos(monitors[i], &x, &y);
			vmode = glfwGetVideoMode(monitors[i]);
			if (currentPosition.x >= x && currentPosition.x < x + vmode->width)
				if (currentPosition.y >= y && currentPosition.y < y + vmode->height)
				{
					relX = currentPosition.x - x;
					relY = currentPosition.y - y;
					break;
				}
		}
		i--;
		glfwGetMonitorPos(monitors[(i >= 0) ? i : count - 1], &x, &y);
		glfwSetWindowPos(Target->GetGLFWWindow(), x + relX, y + relY);
	}
}


MoveToNextMonitor::MoveToNextMonitor(MoveToNextMonitor && o) : WindowAction(std::move(o)) {}
MoveToNextMonitor& MoveToNextMonitor::operator=(MoveToNextMonitor && o) { WindowAction::operator=(std::move(o)); return *this; }
MoveToNextMonitor::MoveToNextMonitor(Window* target) : WindowAction(target) {}
MoveToNextMonitor::~MoveToNextMonitor() {}
void MoveToNextMonitor::Perform()
{
	int count, x, y;
	auto monitors = glfwGetMonitors(&count);
	if (count > 1)
	{
		auto currentPosition = Target->GetPosition();
		auto vmode = glfwGetVideoMode(monitors[0]);
		glfwGetMonitorPos(monitors[0], &x, &y);

		int i = 0;
		int relX = currentPosition.x - x;
		int relY = currentPosition.y - y;
		for (; i < count; i++)
		{
			glfwGetMonitorPos(monitors[i], &x, &y);
			vmode = glfwGetVideoMode(monitors[i]);
			if (currentPosition.x >= x && currentPosition.x < x + vmode->width)
				if (currentPosition.y >= y && currentPosition.y < y + vmode->height)
				{
					relX = currentPosition.x - x;
					relY = currentPosition.y - y;
					break;
				}
		}
		i++;
		glfwGetMonitorPos(monitors[(i >= count) ? 0 : i], &x, &y);
		glfwSetWindowPos(Target->GetGLFWWindow(), x + relX, y + relY);
	}
}