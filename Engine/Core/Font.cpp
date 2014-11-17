#include "Font.h"

namespace Core
{

	Font::Font(Core::Texture* texture, float charWidth, float charHeight, int charsPerRow, float spacingWidth, float spacingHeight)
	{
		Texture = texture;
		CharWidth = charWidth;
		CharHeight = charHeight;
		CharsPerRow = charsPerRow;
		SpacingWidth = spacingWidth;
		SpacingHeight = spacingHeight;
	}


	Font::~Font()
	{
	}

}