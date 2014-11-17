#pragma once
#include "Core.h"
#include "Texture.h"

namespace Core
{
	class Font
	{
	public:
		Texture* Texture;
		float CharWidth;
		float CharHeight;
		int CharsPerRow;
		float SpacingWidth;
		float SpacingHeight;

		Font(Core::Texture* texture, float charWidth, float charHeight, int charsPerRow, float spacingWidth, float spacingHeight);
		~Font();
	};
}