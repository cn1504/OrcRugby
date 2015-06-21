#include "Label.h"
#include <Assets/AssetDB.h>
#include <Assets/Font.h>

using namespace Core::Components::Gui;


Label::Label(std::string font, const glm::vec4& color, std::string text)
{
	Font = font;
	Color = color;
	Text = text;

	Alignment = 2;

	vao = nullptr;
}

Label::Label(std::string font, const glm::vec4& color, int alignment, std::string text)
	: Label(font, color, text)
{
	Alignment = alignment;
}

Label::~Label()
{
}

void Label::Draw(Core::Renderers::GuiRenderer* renderer)
{
	if (Text != "")
	{
		if (vao == nullptr)
			SetText(Text);

		auto f = Core::AssetDB->GetFont(Font);
		if (f != nullptr)
		{
			auto t = Core::AssetDB->GetTexture(f->GetTexture());
			if (t != nullptr)
			{
				renderer->DrawText(*vao, *t, Color, GetMatrix());
			}
		}
	}
	Item::Draw(renderer);
}

void Label::SetText(std::string text)
{
	Text = text;
	if (Text == "")
		return;
	auto f = Core::AssetDB->GetFont(Font);

	size_t longestLength = 0;
	auto lines = std::split(Text, '\n');
	for (auto& line : lines)
	{
		if (line.size() > longestLength)
			longestLength = line.size();
	}
	glm::vec2 size = 0.5f * glm::vec2(longestLength * f->GetCharWidth(),
		lines.size() * f->GetCharHeight());
	Scale(1.0f / GetScale());
	Scale(size * 2.0f);


	std::vector<float> vertices;

	float y = size.y - f->GetCharHeight();

	for (size_t line = 0; line < lines.size(); line++)
	{
		float linelength = lines[line].size() * f->GetCharWidth();
		float x = (Alignment == 1) ? -size.x : (Alignment == 2) ? -linelength * 0.5f : size.x - linelength;

		for (size_t i = 0; i < lines[line].size(); i++)
		{

			// get ascii code as integer
			int ascii = lines[line][i];

			int atlas_col = (ascii - ' ') % f->GetCharsPerRow();
			int atlas_row = (ascii - ' ') / f->GetCharsPerRow();

			// work out texture coordinates in atlas
			float offset_s = (atlas_col * f->GetCharWidth()) / (f->GetCharWidth() * f->GetCharsPerRow());
			float offset_t = (atlas_row * f->GetCharHeight()) / (f->GetCharHeight() * f->GetCharsPerColumn());



			glm::vec2 pos(x, y);
			glm::vec2 dim(f->GetCharWidth(), f->GetCharHeight());


			vertices.push_back(pos.x / size.x);
			vertices.push_back(pos.y / size.y);
			vertices.push_back(offset_s);
			vertices.push_back(offset_t + (1.0f / ((float)f->GetCharsPerColumn())));

			vertices.push_back((pos.x + dim.x) / size.x);
			vertices.push_back(pos.y / size.y);
			vertices.push_back(offset_s + (1.0f / ((float)f->GetCharsPerRow())));
			vertices.push_back(offset_t + (1.0f / ((float)f->GetCharsPerColumn())));

			vertices.push_back(pos.x / size.x);
			vertices.push_back((pos.y + dim.y) / size.y);
			vertices.push_back(offset_s);
			vertices.push_back(offset_t);

			vertices.push_back(pos.x / size.x);
			vertices.push_back((pos.y + dim.y) / size.y);
			vertices.push_back(offset_s);
			vertices.push_back(offset_t);

			vertices.push_back((pos.x + dim.x) / size.x);
			vertices.push_back(pos.y / size.y);
			vertices.push_back(offset_s + (1.0f / ((float)f->GetCharsPerRow())));
			vertices.push_back(offset_t + (1.0f / ((float)f->GetCharsPerColumn())));

			vertices.push_back((pos.x + dim.x) / size.x);
			vertices.push_back((pos.y + dim.y) / size.y);
			vertices.push_back(offset_s + (1.0f / ((float)f->GetCharsPerRow())));
			vertices.push_back(offset_t);

			x = x + f->GetCharWidth();
		}

		y = y - f->GetCharHeight();
	}

	// Update gpu memory
	if (vao == nullptr)
		vao = std::make_unique<Core::Renderers::VertexArray>();

	vao->SetBuffer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * vertices.size(), &vertices[0]);
}