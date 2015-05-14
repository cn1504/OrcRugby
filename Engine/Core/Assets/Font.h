#pragma once
#include <Core.h>
#include "Texture.h"

namespace Core
{
	namespace Assets
	{
		class Font
		{
		private:
			std::string Texture;
			float CharWidth;
			float CharHeight;
			int CharsPerRow;
			int CharsPerColumn;

		public:
			Font(std::string texture, float charWidth, float charHeight, int charsPerRow, int charsPerColumn)
				: Texture(texture), CharWidth(charWidth), CharHeight(charHeight), CharsPerRow(charsPerRow), CharsPerColumn(charsPerColumn)
			{}
			~Font() {}

			std::string GetTexture() const { return Texture; }
			float GetCharWidth() const { return CharWidth; }
			float GetCharHeight() const { return CharHeight; }
			int GetCharsPerRow() const { return CharsPerRow; }
			int GetCharsPerColumn() const { return CharsPerColumn; }
		};
	}
}