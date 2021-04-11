#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh()
		: mVao(0)
		, mVbo(0)
		, mModelMatrix()
	{
	}

	~Mesh()
	{
		if (mVao != 0)
		{
			glDeleteVertexArrays(1, &mVao);
			mVao = 0;
		}
		if (mVbo != 0)
		{
			glDeleteBuffers(1, &mVbo);
			mVbo = 0;
		}
	}

	void Draw()
	{
	}

	unsigned int mVao;
	unsigned int mVbo;

	glm::mat4 mModelMatrix;
};