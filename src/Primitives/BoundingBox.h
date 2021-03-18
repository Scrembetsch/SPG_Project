#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Util/Ray.h"
#include "BoundingBox.h"

class BoundingBox
{
private:
	static const int FLOAT_ARRAY_SIZE = 24;

	enum Pos
	{
		LEFT_DOWN_FRONT = 0,
		RIGHT_DOWN_FRONT = 3,
		RIGHT_DOWN_BACK = 6,
		LEFT_DOWN_BACK = 9,
		LEFT_UP_FRONT = 12,
		RIGHT_UP_FRONT = 15,
		RIGHT_UP_BACK = 18,
		LEFT_UP_BACK = 21
	};

public:
	BoundingBox()
		: mMin(999999)
		, mMax(-999999)
		, mLineVAO(0)
		, mLoopVAO(0)
		, mLineVBO(0)
		, mLoopVBO(0)
	{
		mMin.w = 1;
		mMax.w = 1;
		UpdateLineArray();
	}

	~BoundingBox()
	{
	}

	Node::Axis LargestAxis()
	{
		glm::vec3 d = mMax - mMin;
		if (d.x > d.y && d.x > d.z)
		{
			return Node::Axis::X;
		}
		else if (d.y > d.z)
		{
			return Node::Axis::Y;
		}
		else
		{
			return Node::Axis::Z;
		}
	}

	void AddVertex(const glm::vec3& vertex, bool updateBox)
	{
		mMin.x = glm::min(mMin.x, vertex.x);
		mMin.y = glm::min(mMin.y, vertex.y);
		mMin.z = glm::min(mMin.z, vertex.z);

		mMax.x = glm::max(mMax.x, vertex.x);
		mMax.y = glm::max(mMax.y, vertex.y);
		mMax.z = glm::max(mMax.z, vertex.z);

		if (updateBox)
		{
			UpdateLineArray();
		}
	}

	void Clear()
	{
		mMin.x = 999999;
		mMin.y = 999999;
		mMin.z = 999999;

		mMax.x = -999999;
		mMax.y = -999999;
		mMax.z = -999999;
		UpdateLineArray();
	}

	void Draw()
	{
		//glLineWidth(5);
		glBindVertexArray(mLoopVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glDrawArrays(GL_LINE_LOOP, 4, 4);

		glBindVertexArray(mLineVAO);
		glDrawArrays(GL_LINES, 0, 8);
		glBindVertexArray(0);
	}

	void UpdateLineArray()
	{
		if (mLoopVAO != 0)
		{
			glDeleteVertexArrays(1, &mLoopVAO);
			mLoopVAO = 0;
		}
		if (mLoopVBO != 0)
		{
			glDeleteBuffers(1, &mLoopVBO);
			mLoopVBO = 0;
		}

		if (mLineVAO != 0)
		{
			glDeleteVertexArrays(1, &mLineVAO);
			mLineVAO = 0;
		}
		if (mLineVBO != 0)
		{
			glDeleteBuffers(1, &mLineVBO);
			mLineVBO = 0;
		}

		glm::vec4 modMin = mMin;
		glm::vec4 modMax = mMax;

		mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::X] = modMin.x;
		mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::Y] = modMin.y;
		mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::Z] = modMax.z;

		mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::X] = modMax.x;
		mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::Y] = modMin.y;
		mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::Z] = modMax.z;

		mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::X] = modMax.x;
		mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::Y] = modMin.y;
		mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::Z] = modMin.z;

		mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::X] = modMin.x;
		mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::Y] = modMin.y;
		mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::Z] = modMin.z;

		mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::X] = modMin.x;
		mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::Y] = modMax.y;
		mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::Z] = modMax.z;

		mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::X] = modMax.x;
		mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::Y] = modMax.y;
		mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::Z] = modMax.z;

		mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::X] = modMax.x;
		mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::Y] = modMax.y;
		mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::Z] = modMin.z;

		mLineLoopVertices[LEFT_UP_BACK + Node::Axis::X] = modMin.x;
		mLineLoopVertices[LEFT_UP_BACK + Node::Axis::Y] = modMax.y;
		mLineLoopVertices[LEFT_UP_BACK + Node::Axis::Z] = modMin.z;

		mLineVertices[0 + Node::Axis::X] = mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::X];
		mLineVertices[0 + Node::Axis::Y] = mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::Y];
		mLineVertices[0 + Node::Axis::Z] = mLineLoopVertices[LEFT_DOWN_FRONT + Node::Axis::Z];

		mLineVertices[3 + Node::Axis::X] = mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::X];
		mLineVertices[3 + Node::Axis::Y] = mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::Y];
		mLineVertices[3 + Node::Axis::Z] = mLineLoopVertices[LEFT_UP_FRONT + Node::Axis::Z];

		mLineVertices[6 + Node::Axis::X] = mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::X];
		mLineVertices[6 + Node::Axis::Y] = mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::Y];
		mLineVertices[6 + Node::Axis::Z] = mLineLoopVertices[RIGHT_DOWN_FRONT + Node::Axis::Z];

		mLineVertices[9 + Node::Axis::X] = mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::X];
		mLineVertices[9 + Node::Axis::Y] = mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::Y];
		mLineVertices[9 + Node::Axis::Z] = mLineLoopVertices[RIGHT_UP_FRONT + Node::Axis::Z];

		mLineVertices[12 + Node::Axis::X] = mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::X];
		mLineVertices[12 + Node::Axis::Y] = mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::Y];
		mLineVertices[12 + Node::Axis::Z] = mLineLoopVertices[LEFT_DOWN_BACK + Node::Axis::Z];

		mLineVertices[15 + Node::Axis::X] = mLineLoopVertices[LEFT_UP_BACK + Node::Axis::X];
		mLineVertices[15 + Node::Axis::Y] = mLineLoopVertices[LEFT_UP_BACK + Node::Axis::Y];
		mLineVertices[15 + Node::Axis::Z] = mLineLoopVertices[LEFT_UP_BACK + Node::Axis::Z];

		mLineVertices[18 + Node::Axis::X] = mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::X];
		mLineVertices[18 + Node::Axis::Y] = mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::Y];
		mLineVertices[18 + Node::Axis::Z] = mLineLoopVertices[RIGHT_DOWN_BACK + Node::Axis::Z];

		mLineVertices[21 + Node::Axis::X] = mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::X];
		mLineVertices[21 + Node::Axis::Y] = mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::Y];
		mLineVertices[21 + Node::Axis::Z] = mLineLoopVertices[RIGHT_UP_BACK + Node::Axis::Z];

		glGenVertexArrays(1, &mLoopVAO);
		glGenBuffers(1, &mLoopVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, mLoopVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * FLOAT_ARRAY_SIZE, mLineLoopVertices, GL_STATIC_DRAW);
		// Link vertex attributes for shader
		glBindVertexArray(mLoopVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenVertexArrays(1, &mLineVAO);
		glGenBuffers(1, &mLineVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * FLOAT_ARRAY_SIZE, mLineVertices, GL_STATIC_DRAW);
		// Link vertex attributes for shader
		glBindVertexArray(mLineVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	bool Intersect(const Ray& ray)
	{
		float tmin = (mMin.x - ray.mOrigin.x) / ray.mDirection.x;
		float tmax = (mMax.x - ray.mOrigin.x) / ray.mDirection.x;

		if (tmin > tmax) std::swap(tmin, tmax);

		float tymin = (mMin.y - ray.mOrigin.y) / ray.mDirection.y;
		float tymax = (mMax.y - ray.mOrigin.y) / ray.mDirection.y;

		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (mMin.z - ray.mOrigin.z) / ray.mDirection.z;
		float tzmax = (mMax.z - ray.mOrigin.z) / ray.mDirection.z;

		if (tzmin > tzmax) std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}

private:
	glm::vec4 mMin;
	glm::vec4 mMax;

	float mLineLoopVertices[FLOAT_ARRAY_SIZE];
	float mLineVertices[FLOAT_ARRAY_SIZE];
	unsigned int mLoopVAO;
	unsigned int mLoopVBO;
	unsigned int mLineVAO;
	unsigned int mLineVBO;
};