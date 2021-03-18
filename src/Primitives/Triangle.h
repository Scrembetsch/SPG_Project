#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "Node.h"

class Triangle : public Node
{
private:
	static const int MAX_VERTICES = 3;
	static const int BUFFER_SIZE = MAX_VERTICES * 3 * 3 * 2;

public:


	Triangle()
		:Triangle
		(
			new glm::vec4[]
			{
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
			},
			new glm::vec3[]
			{
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			},
			new glm::vec2[]
			{
				glm::vec2(0.0f, 0.0f),
				glm::vec2(0.0f, 1.0f),
				glm::vec2(0.0f, 1.0f)
			}
		)
	{	
	}

	Triangle(glm::vec4* vertices, glm::vec3* normals, glm::vec2* texCoords)
		: Node()
		, mVBO(0)
		, mVAO(0)
		, mVertices(vertices)
		, mNormals(normals)
		, mTexCoords(texCoords)
		, mGlVertices(new float[BUFFER_SIZE])
		, mIsHit(false)
		, mHitChecked(false)
	{
		SetupArrayBuffer();
	}

	~Triangle()
	{
		Node::~Node();
		if (mVAO != 0)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
		if (mVBO != 0)
		{
			glDeleteBuffers(1, &mVBO);
		}
		delete[] mVertices;
		delete[] mNormals;
		delete[] mTexCoords;
		delete[] mGlVertices;
	}

	void UpdateArrayBuffers()
	{
		SetupArrayBuffer();
	}

	void Draw(Material* customMaterial = nullptr) override
	{
		if (customMaterial != nullptr)
		{
			customMaterial->use();
			//customMaterial->GetShader()->setMat4(mModelMatrixParam, mModelMatrix);
		}
		else
		{
			mMaterial->use();
			//mMaterial->GetShader()->setMat4(mModelMatrixParam, mModelMatrix);
		}
		if (mIsHit && customMaterial == nullptr)
		{
			mMaterial->GetShader()->setVec3("uColor", glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		if (mIsHit && customMaterial == nullptr)
		{
			mMaterial->GetShader()->setVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}

	void SetModelMatrix(const glm::mat4& matrix)
	{
		Node::SetModelMatrix(matrix);
		for (int i = 0; i < MAX_VERTICES; i++)
		{
			mVertices[i] = matrix * mVertices[i];
		}
		UpdateArrayBuffers();
	}

	bool Intersects(const Ray& ray, float& hitT)
	{
		mHitChecked = true;

		glm::vec3 v0v1 = glm::vec3(mVertices[1]) - glm::vec3(mVertices[0]);
		glm::vec3 v0v2 = glm::vec3(mVertices[2]) - glm::vec3(mVertices[0]);
		glm::vec3 pvec = glm::cross(ray.mDirection, v0v2);
		float det = glm::dot(v0v1, pvec);

		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < Ray::GetEpsilon()) return false;

		float invDet = 1 / det;

		glm::vec3 tvec = ray.mOrigin - glm::vec3(mVertices[0]);
		float u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		glm::vec3 qvec = glm::cross(tvec, v0v1);
		float v = glm::dot(ray.mDirection, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		hitT =  glm::dot(v0v2, qvec) * invDet;
		mIsHit = true;
		return true;
	}

	glm::vec4* mVertices;
	glm::vec3* mNormals;
	glm::vec2* mTexCoords;

	bool mIsHit;
	bool mHitChecked;
protected:
	float* mGlVertices;
	unsigned int mVBO;
	unsigned int mVAO;

	void SetupArrayBuffer()
	{
		// Delete old values
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

		for (int i = 0; i < MAX_VERTICES; i++)
		{
			glm::vec3 temp = mVertices[i];
			mGlVertices[0 + i * 8] = temp.x;
			mGlVertices[1 + i * 8] = temp.y;
			mGlVertices[2 + i * 8] = temp.z;

			mGlVertices[3 + i * 8] = mNormals[i].x;
			mGlVertices[4 + i * 8] = mNormals[i].y;
			mGlVertices[5 + i * 8] = mNormals[i].z;

			mGlVertices[6 + i * 8] = mTexCoords[i].x;
			mGlVertices[7 + i * 8] = mTexCoords[i].y;
		}

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BUFFER_SIZE, mGlVertices, GL_STATIC_DRAW);
		// Link vertex attributes for shader
		glBindVertexArray(mVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};