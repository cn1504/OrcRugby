#include "LogPanel.h"
#include <Components/Log.h>
#include <Components/Gui/Anchored.h>

using namespace Game::Components::Gui;


LogPanel::LogPanel() 
	: LogPanel("")
{
}

LogPanel::LogPanel(std::string background)
	: Panel(background)
{
	LinePosition = 0;
	LinesDisplayed = 0;

	Text = std::make_shared<Core::Components::Gui::Label>("Consolas16", glm::vec4(1, 1, 1, 1), 1, "Log:");
	Text = std::make_shared<Core::Components::Gui::Anchored>(Text, std::make_unique<Core::Components::Gui::AlignTopLeft>(glm::vec2(6, 0)));
	AddChild(Text);

	ObserverRegistered = false;
}

LogPanel::~LogPanel()
{
	Text = nullptr;
}

void LogPanel::Notify()
{
	UpdateText();	
}

void LogPanel::UpdateText()
{
	if (LinePosition + LinesDisplayed < Game::Log->GetSize())
		LinePosition++;
	std::string lines = "\n";
	for (size_t i = LinePosition; i < Game::Log->GetSize() && i < LinePosition + LinesDisplayed; i++)
	{
		auto line = Game::Log->GetLine(i);
		while (line.length() > CharsPerRow)
		{
			auto p = line.find_last_of(" ", CharsPerRow);
			lines += line.substr(0, p) + "\n";
			line = line.substr(p + 1, line.npos);
		}
		lines += line + "\n";
	}
	Core::Debug->Log(lines);
	if (lines.length() > 0)
	{
		size_t p = lines.length();
		p = lines.find_last_of("\n");
		size_t i = 0;
		std::string output;
		for (; i < LinesDisplayed; i++)
			p = lines.find_last_of("\n", p - 1);
		for (; i < LinesDisplayed; i++)
			output += "\n";
		if (p != std::string::npos)
		{
			Core::Debug->Log(std::to_string(p));
			output += lines.substr(p + 1);
		}
		lines = output;
	}
	Core::Debug->Log(lines);
	Text->SetText(lines);
}

void LogPanel::Scale(const glm::vec2& delta)
{
	Panel::Scale(delta);

	const float LINE_HEIGHT = 16.0f;
	const float LINE_WIDTH = 9.0f;

	auto s = GetScale();
	LinesDisplayed = static_cast<size_t>(s.y / LINE_HEIGHT);
	CharsPerRow = static_cast<size_t>(s.x / LINE_WIDTH);
	UpdateText();
}

void LogPanel::Update()
{
	Panel::Update();

	if (!ObserverRegistered)
	{
		Game::Log->AddObserver(std::dynamic_pointer_cast<LogPanel>(shared_from_this()));
		ObserverRegistered = true;
	}
}