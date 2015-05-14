#pragma once
#include <Core.h>
#include "Action.h"

namespace Core
{
	namespace Input
	{
		class Map
		{
		private:
			std::unordered_map<std::string, std::vector<std::weak_ptr<Action>>> OnPress;
			std::unordered_map<std::string, std::vector<std::weak_ptr<Action>>> OnRelease;
			std::unordered_map<std::string, std::vector<std::weak_ptr<Action>>> WhileDown;

			std::string MapKey(int key);
			void ProcessInputString(std::string value, int action);

			std::vector<std::string> Down;

			glm::vec2 mousePosition;
			glm::vec2 mouseDelta;

		public:
			const glm::vec2& MousePosition;
			const glm::vec2& MouseDelta;

			Map();
			~Map();

			void AddPressAction(std::string key, std::weak_ptr<Action> action);
			void AddReleaseAction(std::string key, std::weak_ptr<Action> action);
			void AddWhileDownAction(std::string key, std::weak_ptr<Action> action);

			void KeyEvent(int key, int scancode, int action, int mods);
			void MouseButtonEvent(int button, int action, int mods);
			void ScrollEvent(double x, double y);
			void Update();
		};
	}
}