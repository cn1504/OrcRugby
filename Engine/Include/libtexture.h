#pragma once

#include <string>
#include <vector>

namespace LibTexture
{
	unsigned LoadPNG(std::vector<unsigned char>& image, unsigned int width, unsigned int height, const std::string& pngFilename);
}