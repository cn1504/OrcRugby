#include "Map.h"
#include <Window/Window.h>

using namespace Core::Input;

Map::Map() : MousePosition(mousePosition), MouseDelta(mouseDelta) {}
Map::~Map() {}

void Map::AddPressAction(std::string key, std::weak_ptr<Action> action)
{
	OnPress[key].push_back(action);
}

void Map::AddReleaseAction(std::string key, std::weak_ptr<Action> action)
{
	OnRelease[key].push_back(action);
}

void Map::AddWhileDownAction(std::string key, std::weak_ptr<Action> action)
{
	WhileDown[key].push_back(action);
}

void Map::KeyEvent(int key, int scancode, int action, int mods)
{
	std::string value; 
	if (mods & GLFW_MOD_SHIFT && key != GLFW_KEY_LEFT_SHIFT && key != GLFW_KEY_RIGHT_SHIFT)
	{
		value += "Shift + ";
	}
	if (mods & GLFW_MOD_CONTROL && key != GLFW_KEY_LEFT_CONTROL && key != GLFW_KEY_RIGHT_CONTROL)
	{
		value += "Ctrl + ";
	}
	if (mods & GLFW_MOD_ALT && key != GLFW_KEY_LEFT_ALT && key != GLFW_KEY_RIGHT_ALT)
	{
		value += "Alt + ";
	}	
	value += MapKey(key);
	
	ProcessInputString(value, action);
}

void Map::MouseButtonEvent(int button, int action, int mods)
{
	std::string value;
	if (mods & GLFW_MOD_SHIFT)
	{
		value += "Shift + ";
	}
	if (mods & GLFW_MOD_CONTROL)
	{
		value += "Ctrl + ";
	}
	if (mods & GLFW_MOD_ALT)
	{
		value += "Alt + ";
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		value += "Mouse Left";
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		value += "Mouse Right";
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		value += "Mouse Middle";
	}
	else 
	{
		value += "Mouse Button " + std::to_string(button);
	}

	ProcessInputString(value, action);
}

void Map::ScrollEvent(double x, double y)
{
	std::string value;
	if (y > 0)
	{
		value += "Scroll Down";
	}
	else if (y < 0)
	{
		value += "Scroll Up";
	}
	else if (x > 0)
	{
		value += "Scroll Right";
	}
	else if (x < 0)
	{
		value += "Scroll Left";
	}

	ProcessInputString(value, GLFW_RELEASE);
}

void Map::Update()
{
	for (int v = 0; v < Down.size(); v++)
	{
		auto i = WhileDown.count(Down[v]);
		if (i > 0)
		{
			auto vec = &((WhileDown)[Down[v]]);
			for (int j = 0; j < vec->size(); j++)
			{
				if (auto c = vec->at(j).lock())
				{
					c->Perform();
				}
				else
				{
					(*vec)[j] = vec->back();
					vec->pop_back();
					j--;
				}
			}
		}
	}

	double x, y;
	glfwGetCursorPos(Core::Window::CurrentContext->GetGLFWWindow(), &x, &y);
	mouseDelta = glm::vec2((float)x - mousePosition.x, (float)y - mousePosition.y);
	mousePosition = glm::vec2(x, y);
}

void Map::ProcessInputString(std::string value, int action)
{
	if (action == GLFW_PRESS)
	{
		Down.push_back(value);
	}
	else if (action == GLFW_RELEASE)
	{
		for (int i = 0; i < Down.size(); i++)
		{
			if (Down[i] == value)
			{
				std::swap(Down[i], Down.back());
				Down.pop_back();
				i--;
			}
		}
	}

	auto map = (action == GLFW_PRESS) ? &OnPress : (action == GLFW_RELEASE) ? &OnRelease : nullptr;
	if (map != nullptr)
	{
		auto i = map->count(value);
		if (i > 0)
		{
			auto vec = &((*map)[value]);
			for (int j = 0; j < vec->size(); j++)
			{
				if (auto c = vec->at(j).lock())
				{
					c->Perform();
				}
				else
				{
					(*vec)[j] = vec->back();
					vec->pop_back();
					j--;
				}
			}
		}
	}
}

std::string Map::MapKey(int key)
{
	if (key > 33 && key < 96) { std::string result; result.push_back((char)key); return result; }

	if (key == GLFW_KEY_RIGHT) { return "Right"; }
	if (key == GLFW_KEY_LEFT) { return "Left"; }
	if (key == GLFW_KEY_DOWN) { return "Down"; }
	if (key == GLFW_KEY_UP) { return "Up"; }
	if (key == GLFW_KEY_SPACE) { return "Space"; }
	if (key == GLFW_KEY_ESCAPE) { return "Escape"; }
	if (key == GLFW_KEY_ENTER) { return "Enter"; }
	if (key == GLFW_KEY_TAB) { return "Tab"; }
	if (key == GLFW_KEY_BACKSPACE) { return "BackSpace"; }

	if (key == GLFW_KEY_LEFT_SHIFT) { return "Left Shift"; }
	if (key == GLFW_KEY_LEFT_CONTROL) { return "Left Control"; }
	if (key == GLFW_KEY_LEFT_ALT) { return "Left Alt"; }
	if (key == GLFW_KEY_RIGHT_SHIFT) { return "Right Shift"; }
	if (key == GLFW_KEY_RIGHT_CONTROL) { return "Right Control"; }
	if (key == GLFW_KEY_RIGHT_ALT) { return "Right Alt"; }

	if (key == GLFW_KEY_INSERT) { return "Insert"; }
	if (key == GLFW_KEY_DELETE) { return "Delete"; }
	if (key == GLFW_KEY_PAGE_UP) { return "PageUp"; }
	if (key == GLFW_KEY_PAGE_DOWN) { return "PageDown"; }
	if (key == GLFW_KEY_HOME) { return "Home"; }
	if (key == GLFW_KEY_END) { return "End"; }
	if (key == GLFW_KEY_F1) { return "F1"; }
	if (key == GLFW_KEY_F2) { return "F2"; }
	if (key == GLFW_KEY_F3) { return "F3"; }
	if (key == GLFW_KEY_F4) { return "F4"; }
	if (key == GLFW_KEY_F5) { return "F5"; }
	if (key == GLFW_KEY_F6) { return "F6"; }
	if (key == GLFW_KEY_F7) { return "F7"; }
	if (key == GLFW_KEY_F8) { return "F8"; }
	if (key == GLFW_KEY_F9) { return "F9"; }
	if (key == GLFW_KEY_F10) { return "F10"; }
	if (key == GLFW_KEY_F11) { return "F11"; }
	if (key == GLFW_KEY_F12) { return "F12"; }
	if (key == GLFW_KEY_KP_0) { return "KeyPad0"; }
	if (key == GLFW_KEY_KP_1) { return "KeyPad1"; }
	if (key == GLFW_KEY_KP_2) { return "KeyPad2"; }
	if (key == GLFW_KEY_KP_3) { return "KeyPad3"; }
	if (key == GLFW_KEY_KP_4) { return "KeyPad4"; }
	if (key == GLFW_KEY_KP_5) { return "KeyPad5"; }
	if (key == GLFW_KEY_KP_6) { return "KeyPad6"; }
	if (key == GLFW_KEY_KP_7) { return "KeyPad7"; }
	if (key == GLFW_KEY_KP_8) { return "KeyPad8"; }
	if (key == GLFW_KEY_KP_9) { return "KeyPad9"; }
	if (key == GLFW_KEY_KP_DIVIDE) { return "KeyPad/"; }
	if (key == GLFW_KEY_KP_MULTIPLY) { return "KeyPad*"; }
	if (key == GLFW_KEY_KP_SUBTRACT) { return "KeyPad-"; }
	if (key == GLFW_KEY_KP_ADD) { return "KeyPad+"; }
	if (key == GLFW_KEY_KP_ENTER) { return "KeyPadEnter"; }
	if (key == GLFW_KEY_KP_DECIMAL) { return "KeyPad."; }
	return "";
}