#include "ParticleSystem.h"
#include "Particle.h"
#include "../Util/Util.h"
#include <glm/gtc/matrix_transform.hpp>
#include <assert.h>

ParticleSystem::ParticleSystem()
	: mInitalized(false)
	, mCurrentReadBuffer(0)
	, mTransformFeedbackBuffer(0)
	, mQuery(0)
	, mTexture(nullptr)
{
	mVbos[0] = 0;
	mVaos[0] = 0;
}

ParticleSystem::~ParticleSystem()
{
	if (mTransformFeedbackBuffer != 0)
	{
		glDeleteTransformFeedbacks(1, &mTransformFeedbackBuffer);
	}
	if (mQuery != 0)
	{
		glDeleteQueries(1, &mQuery);
	}
	if (mVbos[0] != 0)
	{
		glDeleteBuffers(sBufferSize, mVbos);
	}
	if (mVaos[0] != 0)
	{
		glDeleteVertexArrays(sBufferSize, mVaos);
	}
	delete mTexture;
}

bool ParticleSystem::InitParticleSystem(const std::string& updateVertShader, const std::string& updateGeoShader, const std::string& renderVertShader, const std::string& renderGeoShader, const std::string& renderFragShader)
{
	assert(!mInitalized && "Particle system already initalized!");

	const char* sVaryings[] = {
		"vPositionOut",
		"vVelocityOut",
		"vColorOut",
		"vLifeTimeOut",
		"vSizeOut",
		"vTypeOut"
	};
	unsigned int varyingSize = sizeof(sVaryings) / sizeof(sVaryings[0]);
	mUpdateShader.load(updateVertShader.c_str(), nullptr, updateGeoShader.c_str(), false);
	mRenderShader.load(renderVertShader.c_str(), renderFragShader.c_str(), renderGeoShader.c_str());

	for(int i = 0; i < varyingSize; i++)
	{
		glTransformFeedbackVaryings(mUpdateShader.ID, varyingSize, sVaryings, GL_INTERLEAVED_ATTRIBS);
	}
	mUpdateShader.link();

	glGenTransformFeedbacks(1, &mTransformFeedbackBuffer);
	glGenQueries(1, &mQuery);

	glGenBuffers(sBufferSize, mVbos);
	glGenVertexArrays(sBufferSize, mVaos);

	Particle initParticle;
	initParticle.Type = ParticleType::GENERATOR;

	for (int i = 0; i < sBufferSize; i++)
	{
		glBindVertexArray(mVaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mVbos[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * sMaxParticles, nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &initParticle);

		for (unsigned int j = 0; j < varyingSize; j++)
		{
			glEnableVertexAttribArray(j);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)24);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)36);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)40);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)44);
	}
	mCurrentReadBuffer = 0;
	mNumParticles = 1;

	mInitalized = true;
	return true;
}

float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	return fMin + fAdd * fRandom;
}

void ParticleSystem::UpdateParticles(float timeStep)
{
	CheckInit();
	mUpdateShader.use();

	glm::vec3 upload;
	mUpdateShader.setVec3("uPosition",		mPosition);
	mUpdateShader.setVec3("uVelocityMin",	mVelocityMin);
	mUpdateShader.setVec3("uVelocityRange", mVelocityRange);
	mUpdateShader.setVec3("uColor",			mColor);
	mUpdateShader.setVec3("uGravity",		mGravity);

	mUpdateShader.setFloat("uTimeStep",		timeStep);
	mUpdateShader.setFloat("uLifeTimeMin",	mLifeTimeMin);
	mUpdateShader.setFloat("uLifeTimeRange",mLifeTimeRange);
	mUpdateShader.setFloat("uSize",			mSize);

	mElapsedTime += timeStep;

	if (mElapsedTime > mNextGenerationTime)
	{
		mUpdateShader.setInt("uNumToGenerate", mNumToGenerate);
		mElapsedTime -= mNextGenerationTime;

		glm::vec3 randomSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
		mUpdateShader.setVec3("uRandomSeed", randomSeed);
	}
	else
	{
		mUpdateShader.setInt("uNumToGenerate", 0);
	}
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbackBuffer);

	glBindVertexArray(mVaos[mCurrentReadBuffer]);
	glEnableVertexAttribArray(1);

	// 1 -  use other buffer
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mVbos[1 - mCurrentReadBuffer]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mQuery);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, mNumParticles);

	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	glGetQueryObjectiv(mQuery, GL_QUERY_RESULT, &mNumParticles);

	// toggle buffer
	mCurrentReadBuffer = 1 - mCurrentReadBuffer;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::RenderParticles()
{
	CheckInit();

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	mRenderShader.use();
	mTexture->use();
	mRenderShader.setMat4("uProjection", mProjection);
	mRenderShader.setMat4("uView", mView);
	mRenderShader.setVec3("uQuad1", mQuad1);
	mRenderShader.setVec3("uQuad2", mQuad2);

	glBindVertexArray(mVaos[mCurrentReadBuffer]);
	glDisableVertexAttribArray(1);

	glDrawArrays(GL_POINTS, 0, mNumParticles);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticleSystem::SetGeneratorProperties(const glm::vec3& position, const glm::vec3& velocityMin, const glm::vec3& velocityMax, const glm::vec3& gravity, const glm::vec3 color, float minLifeTime, float maxLifeTime, float size, float spawnTime, int numToGenerate)
{
	mPosition = position;
	mVelocityMin = velocityMin;
	mVelocityRange = velocityMax - velocityMin;
	mGravity = gravity;
	mColor = color;

	mSize = size;
	mLifeTimeMin = minLifeTime;
	mLifeTimeRange = maxLifeTime - minLifeTime;

	mNextGenerationTime = spawnTime;
	mElapsedTime = 0.0f;

	mNumToGenerate = numToGenerate;
}

void ParticleSystem::SetGeneratorPosition(const glm::vec3& position)
{
	mPosition = position;
}

int ParticleSystem::GetNumParticles() const
{
	return mNumParticles;
}

void ParticleSystem::SetMatrices(const glm::mat4& projection, const glm::mat4& viewMat, const glm::vec3& view, const glm::vec3& upVector)
{
	mProjection = projection;
	mView = viewMat;
	mQuad1 = glm::cross(view, upVector);
	mQuad1 = glm::normalize(mQuad1);
	mQuad2 = glm::cross(view, mQuad1);
	mQuad2 = glm::normalize(mQuad2);
}

void ParticleSystem::CheckInit() const
{
	assert(mInitalized && "Particle system not initalized before use");
}