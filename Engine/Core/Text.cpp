#include "Text.h"
#include "Shader.h"

namespace Core
{

	Text::Text(std::string txt, int x, int y, Core::Font* font, glm::vec4 color)
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

		GenerateGeometry();
	}


	Text::~Text()
	{
		glDeleteBuffers(1, &VertexBuffer);
	}


	void Text::Render(Shader* shader)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Font->Texture->GetID());
		glUniform1i(shader->GetUL("tex"), 0);
		glUniform4fv(shader->GetUL("color"), 1, glm::value_ptr(Color));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(
			0,                                // attribute
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);
		glDrawArrays(GL_TRIANGLES, 0, ((GLsizei)Txt.length() - newLines) * 6);
		glDisableVertexAttribArray(0);
	}


	void Text::UpdateText(std::string txt)
	{
		if (txt != Txt)
		{
			Txt = txt;
			GenerateGeometry();
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
		GenerateGeometry();
	}

	
	void Text::GenerateGeometry()
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
			GLfloat wWidth = (float)Settings::Window::Width;
			GLfloat wHeight = (float)Settings::Window::Height;
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

}