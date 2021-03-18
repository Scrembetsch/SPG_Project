#pragma once

#include "../glBasics/material.h"
#include "../Util/Ray.h"

class Node
{
public:
	enum Axis
	{
		INVALID = -1,
		X = 0,
		Y = 1,
		Z = 2
	};

	Node()
		: mMaterial()
		, mModelMatrix(1.0)
		, mModelMatrixParam("uModel")
		, mDeleteMaterial(false)
	{
	}

	~Node()
	{
		if (mDeleteMaterial)
		{
			delete mMaterial;
		}
		mMaterial = nullptr;
	}

	virtual void Draw(Material* customMaterial = nullptr) = 0;

	virtual void SetMaterial(Material* mat, bool handleDelete)
	{
		if (mDeleteMaterial)
		{
			delete mMaterial;
		}
		mMaterial = mat;
		mDeleteMaterial = handleDelete;
	}

	virtual Material* GetMaterial()
	{
		return mMaterial;
	}

	virtual void SetModelMatrix(const glm::mat4& matrix)
	{
		mModelMatrix = matrix;
	}

	virtual const glm::mat4& GetModelMatrix()
	{
		return mModelMatrix;
	}

	virtual void SetModelMatrixParam(const std::string& param)
	{
		mModelMatrixParam = param;
	}

protected:
	Material* mMaterial;
	glm::mat4 mModelMatrix;
	std::string mModelMatrixParam;

	bool mDeleteMaterial;
};