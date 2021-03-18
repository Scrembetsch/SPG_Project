#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

class DollyController
{
public:
	DollyController();
	~DollyController();

	std::vector<glm::vec3> mPositions;
	std::vector<glm::quat> mDirections;

	glm::quat SQUAD(float customPoint = -1);
	glm::vec3 TCB(float customPoint = -1);
	glm::vec3 ArbTCB(float distance);

	void NextStep(float t);
	void Reset();

	float mT;
	float mC;
	float mB;
	float mCurrentPoint;
	float mCurrentDistance;
	std::vector<float> mDistanceToNext;
	std::vector<float> mDistanceToPoint;

protected:

	float CalcTcbPart(float k, float p0, float p1, float ingoingTangent, float outgoingTangent);
	glm::vec3 CalcTcbPart(float k, float d, glm::vec3 p0, glm::vec3 p1, glm::vec3 ingoingTangent, glm::vec3 outgoingTangent);

	glm::vec3 CalcIngoingTangent(int index_, int index0, int index1);
	glm::vec3 CalcOutgoingTangent(int index0, int index1, int index2);

	glm::vec3 CalcIngoingTangentArb(int index_, int index0, int index1);
	glm::vec3 CalcOutgoingTangentArb(int index_, int index0, int index1);

	glm::vec3 CalcIngoingTangentPart(glm::vec3 p_, glm::vec3 p0, glm::vec3 p1);
	glm::vec3 CalcOutgoingTangentPart(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

	glm::vec3 CalcIngoingTangentPartArb(glm::vec3 p_, glm::vec3 p0, glm::vec3 p1, float dis_, float dis0, float dis1);
	glm::vec3 CalcOutgoingTangentPartArb(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float dis_, float dis0, float dis1);

};
