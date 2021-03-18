#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "KdVertex.h"

#include "../Primitives/Node.h"
#include "../Primitives/BoundingBox.h"

class KdNode
{
public:
	KdNode()
		: mPoint(nullptr)
		, mAxis(Node::Axis::INVALID)
		, mNodePoints(nullptr)
		, mBoundingBox(nullptr)
		, mNode1(nullptr)
		, mNode2(nullptr)
	{
	}

	~KdNode()
	{
		delete mBoundingBox;
		delete mNode1;
		delete mNode2;
	}

	KdVertex* mPoint;
	Node::Axis mAxis;
	std::vector<KdVertex*>* mNodePoints;
	BoundingBox* mBoundingBox;

	KdNode* mNode1;
	KdNode* mNode2;

	void Draw()
	{
		mBoundingBox->Draw();
		if (mAxis != Node::Axis::INVALID)
		{
			mNode1->Draw();
			mNode2->Draw();
		}
	}
};