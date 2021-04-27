#pragma once

#include <glad/glad.h>
#include <vector>
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material()
		: mShader(nullptr)
		, mTextures()
		, mSharedShader(false)
	{
	}

	~Material()
	{
		for (auto tex : mTextures)
		{
			if (!tex->mShared)
			{
				delete tex;
			}
		}
		mTextures.clear();
		if (!mSharedShader)
		{
			delete mShader;
		}
	}

	void use()
	{
		mShader->use();
		for (auto tex : mTextures)
		{
			tex->use();
		}
	}

	void UseSharedShader(Shader* shader)
	{
		if (mShader != nullptr && !mSharedShader)
		{
			delete mShader;
		}
		mSharedShader = true;
		mShader = shader;
	}

	void UseShader(Shader* shader)
	{
		mSharedShader = false;
		mShader = shader;
	}

	Shader* GetShader()
	{
		return mShader;
	}

	std::vector<Texture*> mTextures;

private:
	Shader* mShader;
	bool mSharedShader;
};