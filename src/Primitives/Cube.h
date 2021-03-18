#pragma once

#include <iostream>

#include "Node.h"
#include "Triangle.h"

class Cube : public Node
{
public:
    static const int MAX_TRIANGLES = 12;
    
public:

	Cube()
		: Node()
	{
		mTriangles = new Triangle*[MAX_TRIANGLES];
        for (int i = 0; i < MAX_TRIANGLES; i++)
        {
            mTriangles[i] = new Triangle();
        }
        SetMaterial(new Material(), true);

        // Back face
        ReduceQuadToTriangle(
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f),
            mTriangles[0],
            mTriangles[1]
        );
        // Front face
        ReduceQuadToTriangle(
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 0.0f,  0.0f,  1.0f),
            mTriangles[2],
            mTriangles[3]
        );
        // Left face
        ReduceQuadToTriangle(
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            mTriangles[4],
            mTriangles[5]
        );
        // Right face
        ReduceQuadToTriangle(
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  0.0f,  0.0f),
            mTriangles[6],
            mTriangles[7]
        );
        // Bottom face
        ReduceQuadToTriangle(
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3( 0.0f, -1.0f,  0.0f),
            mTriangles[8],
            mTriangles[9]
        );
        // Top face
        ReduceQuadToTriangle(
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 0.0f,  1.0f,  0.0f),
            mTriangles[10],
            mTriangles[11]
        );
	}

	~Cube()
	{
		for (int i = 0; i < MAX_TRIANGLES; i++)
		{
			delete mTriangles[i];
			mTriangles[i] = nullptr;
		}
		delete[] mTriangles;
        Node::~Node();
	}

	void Draw(Material* customMaterial = nullptr)
	{
        for (int i = 0; i < MAX_TRIANGLES; i++)
        {
            mTriangles[i]->Draw(customMaterial == nullptr ? nullptr : customMaterial);
        }
	}

    void SetMaterial(Material* mat, bool handleDelete)
    {
        for (int i = 0; i < MAX_TRIANGLES; i++)
        {
            mTriangles[i]->SetMaterial(mat, false);
        }
        Node::SetMaterial(mat, handleDelete);
    }

    void SetModelMatrix(const glm::mat4& matrix)
    {
        for (int i = 0; i < MAX_TRIANGLES; i++)
        {
            mTriangles[i]->SetModelMatrix(matrix);
        }
        Node::SetModelMatrix(matrix);
    }

    void SetModelMatrixParam(const std::string& param)
    {
        for (int i = 0; i < MAX_TRIANGLES; i++)
        {
            mTriangles[i]->SetModelMatrixParam(param);
        }
        Node::SetModelMatrixParam(param);
    }

    Triangle** GetTriangles()
    {
        return mTriangles;
    }

protected:
    static void ReduceQuadToTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal, Triangle* t0, Triangle* t1)
    {
        t0->mVertices[0] = glm::vec4(p0.x, p0.y, p0.z, 1.0f);
        t0->mVertices[1] = glm::vec4(p3.x, p3.y, p3.z, 1.0f);
        t0->mVertices[2] = glm::vec4(p1.x, p1.y, p1.z, 1.0f);

        t0->mNormals[0] = normal;
        t0->mNormals[1] = normal;
        t0->mNormals[2] = normal;

        t0->mTexCoords[0] = glm::vec2(0.0f, 0.0f);
        t0->mTexCoords[1] = glm::vec2(1.0f, 1.0f);
        t0->mTexCoords[2] = glm::vec2(1.0f, 0.0f);
        t0->UpdateArrayBuffers();

        t1->mVertices[0] = glm::vec4(p0.x, p0.y, p0.z, 1.0f);
        t1->mVertices[1] = glm::vec4(p2.x, p2.y, p2.z, 1.0f);
        t1->mVertices[2] = glm::vec4(p3.x, p3.y, p3.z, 1.0f);

        t1->mNormals[0] = normal;
        t1->mNormals[1] = normal;
        t1->mNormals[2] = normal;

        t1->mTexCoords[0] = glm::vec2(0.0f, 0.0f);
        t1->mTexCoords[1] = glm::vec2(0.0f, 1.0f);
        t1->mTexCoords[2] = glm::vec2(1.0f, 1.0f);
        t1->UpdateArrayBuffers();
    }

	Triangle** mTriangles;
};