#pragma once
#include <Core.h>
#include <Input/Action.h>

namespace Core
{
	namespace Window
	{
		class WindowAction : public Core::Input::Action
		{
		protected:
			Window* Target;

		public:
			WindowAction() = delete;
			WindowAction(const WindowAction&) = delete;
			WindowAction& operator=(const WindowAction&) = delete;
			WindowAction(WindowAction &&);
			WindowAction& operator=(WindowAction &&);

			WindowAction(Window* target);
			virtual ~WindowAction();
		};



		class DoNothing : public WindowAction
		{
		public:
			DoNothing() = delete;
			DoNothing(const DoNothing&) = delete;
			DoNothing& operator=(const DoNothing&) = delete;
			DoNothing(DoNothing &&);
			DoNothing& operator=(DoNothing &&);

			DoNothing(Window* target);
			virtual ~DoNothing();
			virtual void Perform();
		};



		class SwitchToFullscreen : public WindowAction
		{
		public:
			SwitchToFullscreen() = delete;
			SwitchToFullscreen(const SwitchToFullscreen&) = delete;
			SwitchToFullscreen& operator=(const SwitchToFullscreen&) = delete;
			SwitchToFullscreen(SwitchToFullscreen &&);
			SwitchToFullscreen& operator=(SwitchToFullscreen &&);

			SwitchToFullscreen(Window* target);
			virtual ~SwitchToFullscreen();
			virtual void Perform();
		};



		class SwitchToWindowed : public WindowAction
		{
		public:
			SwitchToWindowed() = delete;
			SwitchToWindowed(const SwitchToWindowed&) = delete;
			SwitchToWindowed& operator=(const SwitchToWindowed&) = delete;
			SwitchToWindowed(SwitchToWindowed &&);
			SwitchToWindowed& operator=(SwitchToWindowed &&);

			SwitchToWindowed(Window* target);
			virtual ~SwitchToWindowed();
			virtual void Perform();
		};



		class Iconify : public WindowAction
		{
		public:
			Iconify() = delete;
			Iconify(const Iconify&) = delete;
			Iconify& operator=(const Iconify&) = delete;
			Iconify(Iconify &&);
			Iconify& operator=(Iconify &&);

			Iconify(Window* target);
			virtual ~Iconify();
			virtual void Perform();
		};



		class TakeFocus : public WindowAction
		{
		public:
			TakeFocus() = delete;
			TakeFocus(const TakeFocus&) = delete;
			TakeFocus& operator=(const TakeFocus&) = delete;
			TakeFocus(TakeFocus &&);
			TakeFocus& operator=(TakeFocus &&);

			TakeFocus(Window* target);
			virtual ~TakeFocus();
			virtual void Perform();
		};



		class Close : public WindowAction
		{
		public:
			Close() = delete;
			Close(const Close&) = delete;
			Close& operator=(const Close&) = delete;
			Close(Close &&);
			Close& operator=(Close &&);

			Close(Window* target);
			virtual ~Close();
			virtual void Perform();
		};



		class MoveToPreviousMonitor : public WindowAction
		{
		public:
			MoveToPreviousMonitor() = delete;
			MoveToPreviousMonitor(const MoveToPreviousMonitor&) = delete;
			MoveToPreviousMonitor& operator=(const MoveToPreviousMonitor&) = delete;
			MoveToPreviousMonitor(MoveToPreviousMonitor &&);
			MoveToPreviousMonitor& operator=(MoveToPreviousMonitor &&);

			MoveToPreviousMonitor(Window* target);
			virtual ~MoveToPreviousMonitor();
			virtual void Perform();
		};



		class MoveToNextMonitor : public WindowAction
		{
		public:
			MoveToNextMonitor() = delete;
			MoveToNextMonitor(const MoveToNextMonitor&) = delete;
			MoveToNextMonitor& operator=(const MoveToNextMonitor&) = delete;
			MoveToNextMonitor(MoveToNextMonitor &&);
			MoveToNextMonitor& operator=(MoveToNextMonitor &&);

			MoveToNextMonitor(Window* target);
			virtual ~MoveToNextMonitor();
			virtual void Perform();
		};
	}
}