#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Ray
{
public:

	Ray()
		: Ray(glm::vec3(), glm::vec3())
	{
	}

	Ray(const glm::vec3& pos, const glm::vec3& dir, float minDis = 0.001f, float maxDis = 999999)
		: mOrigin(pos)
		, mDirection(dir)
		, mMinDistance(minDis)
		, mMaxDistance(maxDis)
		, mHitDistance(500)
		, mVAO(0)
		, mVBO(0)
	{
	}

	~Ray()
	{
		if (mVAO != 0)
		{
			glDeleteVertexArrays(1, &mVAO);
			mVAO = 0;
		}
		if (mVBO != 0)
		{
			glDeleteBuffers(1, &mVBO);
			mVBO = 0;
		}
	}

	glm::vec3 mOrigin;
	glm::vec3 mDirection;
	float mHitDistance;
	float mMinDistance;
	float mMaxDistance;

	static float GetEpsilon()
	{
		return 0.0000001f;
	}

	void UpdateLine()
	{
		if (mVAO != 0)
		{
			glDeleteVertexArrays(1, &mVAO);
			mVAO = 0;
		}
		if (mVBO != 0)
		{
			glDeleteBuffers(1, &mVBO);
			mVBO = 0;
		}

		mLine[0] = mOrigin.x;
		mLine[1] = mOrigin.y;
		mLine[2] = mOrigin.z;

		glm::vec3 t = mOrigin + mDirection * mHitDistance;
		mLine[3] = t.x;
		mLine[4] = t.y;
		mLine[5] = t.z;

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, mLine, GL_STATIC_DRAW);
		// Link vertex attributes for shader
		glBindVertexArray(mVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw()
	{
		if (mVAO != 0)
		{
			//glLineWidth(5);
			glBindVertexArray(mVAO);
			glDrawArrays(GL_LINES, 0, 2);
			glBindVertexArray(0);
		}
	}

protected:
	float mLine[6];
	unsigned int mVAO;
	unsigned int mVBO;
};