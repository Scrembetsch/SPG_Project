#pragma once

#include <glm/glm.hpp>

#include "../Primitives/Triangle.h"

class KdVertex
{
public:

	KdVertex(const glm::vec3 pos, Triangle* parent)
		: mPos(pos)
		, mParentTriangle(parent)
	{
	}

	glm::vec3 mPos;
	Triangle* mParentTriangle;

public:
	static bool CompareX(KdVertex* lhs, KdVertex* rhs)
	{
		return lhs->mPos.x < rhs->mPos.x;
	}

	static bool CompareY(KdVertex* lhs, KdVertex* rhs)
	{
		return lhs->mPos.y < rhs->mPos.y;
	}

	static bool CompareZ(KdVertex* lhs, KdVertex* rhs)
	{
		return lhs->mPos.z < rhs->mPos.z;
	}
};