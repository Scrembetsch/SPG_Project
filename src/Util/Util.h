#pragma once

#include <string>

#include "../glBasics/texture.h"

#include "../Primitives/Triangle.h"

namespace Util
{
	unsigned int LoadTexture(char const* path);
	bool LoadObjFile(std::string path, std::vector<Node*>& triangles, glm::mat4& matrix);
}