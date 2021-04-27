#pragma once

#include <string>

#include "../glBasics/Texture.h"

namespace Util
{
	unsigned int LoadTexture(char const* path);
	void PrintErrors();
}