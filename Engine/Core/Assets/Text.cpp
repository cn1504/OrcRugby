#include "Text.h"
#include <Renderers/Shader.h>

using namespace Core::Assets;


Text::Text(std::string txt, int x, int y, Core::Assets::Font* font, glm::vec4 color)
{
	Txt = txt;
	Font = font;
	Offset.x = (float)x;
	Offset.y = (float)y;
	Position.x = 0;
	Position.y = 0;
	Color = color;

	VertexBuffer = 0;
	glGenBuffers(1, &VertexBuffer);

	BuildGeometry();
}


Text::~Text()
{
	glDeleteBuffers(1, &VertexBuffer);
}


void Text::Render(Core::Renderers::Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Font->Texture->GetID());
	shader->SetUniform("tex", 0);
	shader->SetUniform("color", Color);

	shader->SetVec4Attribute("coord", VertexBuffer);
	glDrawArrays(GL_TRIANGLES, 0, ((GLsizei)Txt.length() - newLines) * 6);
	shader->DisableAttribute("coord");
}


void Text::UpdateText(std::string txt)
{
	if (txt != Txt)
	{
		Txt = txt;
		BuildGeometry();
	}
}


std::string Text::GetText()
{
	return Txt;
}


void Text::SetColor(glm::vec4 color)
{
	Color = color;
}


void Text::Move(glm::vec2 position)
{
	Position = position;
	BuildGeometry();
}

	
void Text::BuildGeometry()
{
	newLines = 0;
	std::vector<GLfloat> vertices;
		
	GLfloat x = Position.x + Offset.x;
	GLfloat y = Position.y + Offset.y;
		
	for (auto& ch : Txt) {

		// get ascii code as integer
		int ascii = ch;

		if (ascii == '\n') {
			y = y + (Font->CharWidth - Font->SpacingHeight);
			x = Position.x + Offset.x;
			newLines++;
			continue;
		}


		int atlas_col = (ascii - ' ') % Font->CharsPerRow;
		int atlas_row = (ascii - ' ') / Font->CharsPerRow;

		// work out texture coordinates in atlas
		float offset_s = atlas_col * (1.0f / ((float)Font->CharsPerRow));
		float offset_t = atlas_row * (1.0f / ((float)Font->CharsPerRow));



		// Convert from pixel counts to opengl coords
		GLfloat wWidth = windowSize.x;
		GLfloat wHeight = windowSize.y;
		GLfloat wHalfWidth = wWidth / 2.0f;
		GLfloat wHalfHeight = wHeight / 2.0f;
			

		glm::vec2 pos((x - wHalfWidth) / wHalfWidth, (wHeight - y - wHalfHeight) / wHalfHeight);
		glm::vec2 dim(Font->CharWidth / wHalfWidth, Font->CharHeight / wHalfHeight);


		vertices.push_back(pos.x);
		vertices.push_back(pos.y - dim.y);
		vertices.push_back(offset_s);
		vertices.push_back(offset_t + (1.0f / ((float)Font->CharsPerRow)));
			
		vertices.push_back(pos.x + dim.x);
		vertices.push_back(pos.y - dim.y);
		vertices.push_back(offset_s + (1.0f / ((float)Font->CharsPerRow)));
		vertices.push_back(offset_t + (1.0f / ((float)Font->CharsPerRow)));

		vertices.push_back(pos.x);
		vertices.push_back(pos.y);
		vertices.push_back(offset_s);
		vertices.push_back(offset_t);

		vertices.push_back(pos.x);
		vertices.push_back(pos.y);
		vertices.push_back(offset_s);
		vertices.push_back(offset_t);

		vertices.push_back(pos.x + dim.x);
		vertices.push_back(pos.y - dim.y);
		vertices.push_back(offset_s + (1.0f / ((float)Font->CharsPerRow)));
		vertices.push_back(offset_t + (1.0f / ((float)Font->CharsPerRow)));

		vertices.push_back(pos.x + dim.x);
		vertices.push_back(pos.y);
		vertices.push_back(offset_s + (1.0f / ((float)Font->CharsPerRow)));
		vertices.push_back(offset_t);

		x = x + (Font->CharWidth - Font->SpacingWidth);

	}

	// Update gpu memory
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
}
