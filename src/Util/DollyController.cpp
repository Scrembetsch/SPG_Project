#include "DollyController.h"

#define DISTANCE_RES 0.01
#define SPLINE_MOVE_RES 0.1

DollyController::DollyController()
	: mT(0)
	, mC(0)
	, mB(0)
	, mCurrentPoint(0)
	, mCurrentDistance(0)
{
}

DollyController::~DollyController()
{
}

glm::quat DollyController::SQUAD(float customPoint)
{
	int currentPoint = static_cast<int>(customPoint < 0 ? mCurrentPoint : customPoint);
	float k = fmodf(customPoint < 0 ? mCurrentPoint : customPoint, 1.0);
	int arraySize = mDirections.size() - 1;

	int index_ = glm::clamp(currentPoint - 1, 0, arraySize);
	int index0 = glm::clamp(currentPoint, 0, arraySize);
	int index1 = glm::clamp(currentPoint + 1, 0, arraySize);
	int index2 = glm::clamp(currentPoint + 2, 0, arraySize);

	if (index0 == index2)
	{
		return mDirections[index0];
	}

	glm::quat dir_ = mDirections[index_];
	glm::quat dir0 = mDirections[index0];
	glm::quat dir1 = mDirections[index1];
	glm::quat dir2 = mDirections[index2];

	glm::quat s1 = glm::intermediate(dir_, dir0, dir1);
	glm::quat s2 = glm::intermediate(dir0, dir1, dir2);

	return glm::squad(dir0, dir1, s1, s2, k);
}

glm::vec3 DollyController::TCB(float customPoint)
{
	int currentPoint = static_cast<int>(customPoint < 0 ? mCurrentPoint : customPoint);
	float k = fmodf(customPoint < 0 ? mCurrentPoint : customPoint, 1.0);
	int arraySize = mPositions.size() - 1;

	int index_ = glm::clamp(currentPoint - 1, 0, arraySize);
	int index0 = glm::clamp(currentPoint, 0, arraySize);
	int index1 = glm::clamp(currentPoint + 1, 0, arraySize);
	int index2 = glm::clamp(currentPoint + 2, 0, arraySize);

	if (index0 == index2)
	{
		return mPositions[index0];
	}

	glm::vec3 ingoingTangent = CalcIngoingTangent(index_, index0, index1);
	glm::vec3 outgoingTangent = CalcOutgoingTangent(index0, index1, index2);

	glm::vec3 ret = CalcTcbPart(k, 1, mPositions[index0], mPositions[index1], ingoingTangent, outgoingTangent);
	return ret;
}

glm::vec3 DollyController::ArbTCB(float distance)
{
	int index = -1;
	float k = 0;
	float d = 0;
	mCurrentDistance = distance;
	for (int i = static_cast<int>(mCurrentPoint); i < static_cast<int>(mDistanceToPoint.size()); i++)
	{
		if (mCurrentDistance <= mDistanceToPoint[i])
		{
			index = i - 1;
			break;
		}
	}
	if (index == -1)
	{
		index = mPositions.size();
	}
	else
	{
		float localDistance = mCurrentDistance - mDistanceToPoint[index];
		k = (mCurrentDistance - mDistanceToPoint[index]) / (mDistanceToPoint[index + 1] - mDistanceToPoint[index]);
		d = mDistanceToPoint[index + 1] - mDistanceToPoint[index];
	}

	int currentPoint = (int)mCurrentPoint;
	int arraySize = mPositions.size() - 1;

	int index_ = glm::clamp(currentPoint - 1, 0, arraySize);
	int index0 = glm::clamp(currentPoint, 0, arraySize);
	int index1 = glm::clamp(currentPoint + 1, 0, arraySize);

	if (index0 == index1)
	{
		return mPositions[index0];
	}

	glm::vec3 ingoingTangent = CalcIngoingTangentArb(index_, index0, index1);
	glm::vec3 outgoingTangent = CalcOutgoingTangentArb(index_, index0, index1);

	glm::vec3 ret = CalcTcbPart(k, d, mPositions[index0], mPositions[index1], ingoingTangent, outgoingTangent);
	return ret;
}

glm::vec3 DollyController::CalcTcbPart(float k, float d, glm::vec3 p0, glm::vec3 p1, glm::vec3 ingoingTangent, glm::vec3 outgoingTangent)
{
	float k2 = k * k;
	float k3 = k2 * k;

	return (((2 * k3) - (3 * k2) + 1) * p0) + (((-2 * k3) + (3 * k2)) * p1) + ((k3 - (2 * k2) + k) * ingoingTangent * d) + ((k3 - k2) * outgoingTangent * d);
}

float DollyController::CalcTcbPart(float k, float p0, float p1, float ingoingTangent, float outgoingTangent)
{
	float k2 = k * k;
	float k3 = k2 * k;

	return (((2 * k3) - (3 * k2) + 1) * p0) + (((-2 * k3) + (3 * k2)) * p1) + ((k3 - (2 * k2) + k) * ingoingTangent) + ((k3 - k2) * outgoingTangent);
}

glm::vec3 DollyController::CalcIngoingTangent(int index_, int index0, int index1)
{
	return CalcIngoingTangentPart(mPositions[index_], mPositions[index0], mPositions[index1]);
}

glm::vec3 DollyController::CalcOutgoingTangent(int index0, int index1, int index2)
{
	return CalcOutgoingTangentPart(mPositions[index0], mPositions[index1], mPositions[index2]);
}

glm::vec3 DollyController::CalcIngoingTangentArb(int index_, int index0, int index1)
{
	return CalcIngoingTangentPartArb(mPositions[index_], mPositions[index0], mPositions[index1], mDistanceToPoint[index_], mDistanceToPoint[index0], mDistanceToPoint[index1]);
}
glm::vec3 DollyController::CalcOutgoingTangentArb(int index_, int index0, int index1)
{
	return CalcOutgoingTangentPartArb(mPositions[index_], mPositions[index0], mPositions[index1], mDistanceToPoint[index_], mDistanceToPoint[index0], mDistanceToPoint[index1]);
}

glm::vec3 DollyController::CalcIngoingTangentPart(glm::vec3 p_, glm::vec3 p0, glm::vec3 p1)
{
	return ((((1 - mT) * (1 + mB) * (1 + mC)) / 2) * (p0 - p_)) + ((((1 - mT) * (1 - mB) * (1 - mC)) / 2) * (p1 - p0));
}

glm::vec3 DollyController::CalcOutgoingTangentPart(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	return ((((1 - mT) * (1 + mB) * (1 - mC)) / 2) * (p1 - p0)) + ((((1 - mT) * (1 - mB) * (1 + mC)) / 2) * (p2 - p1));
}

glm::vec3 DollyController::CalcIngoingTangentPartArb(glm::vec3 p_, glm::vec3 p0, glm::vec3 p1, float dis_, float dis0, float dis1)
{
	return ((((1 - mT) * (1 + mB) * (1 - mC)) / 2) * ((p0 - p_) / (dis0 - dis_))) + ((((1 - mT) * (1 - mB) * (1 + mC)) / 2) * ((p1 - p0) / (dis1 - dis0)));
}

glm::vec3 DollyController::CalcOutgoingTangentPartArb(glm::vec3 p_, glm::vec3 p0, glm::vec3 p1, float dis_, float dis0, float dis1)
{
	return ((((1 - mT) * (1 + mB) * (1 + mC)) / 2) * ((p0 - p_) / (dis0 - dis_))) + ((((1 - mT) * (1 - mB) * (1 - mC)) / 2) * ((p1 - p0) / (dis1 - dis0)));;
}

void DollyController::NextStep(float s)
{
	int index = -1;
	mCurrentDistance += s;
	for (int i = static_cast<int>(mCurrentPoint); i < static_cast<int>(mDistanceToPoint.size()); i++)
	{
		if (mCurrentDistance <= mDistanceToPoint[i])
		{
			index = i - 1;
			break;
		}
	}
	if (index == -1)
	{
		mCurrentPoint = mPositions.size() + 1;
	}
	else
	{
		float localDistance = mCurrentDistance - mDistanceToPoint[index];
		float estSplinePart = localDistance / mDistanceToNext[index];
		mCurrentPoint = index + estSplinePart;
	}
}

void DollyController::Reset()
{
	mCurrentPoint = 0;
	mCurrentDistance = 0;
	mDistanceToNext.clear();
	mDistanceToPoint.clear();
	if (mPositions.size() == 0)
	{
		return;
	}
	for (int i = 0; i < static_cast<int>(mPositions.size()); i++)
	{
		mDistanceToNext.push_back(0);
		mDistanceToPoint.push_back(0);
	}
	glm::vec3 prevPos = mPositions[0];
	for (float k = DISTANCE_RES; k <= mPositions.size() - 1; k += DISTANCE_RES)
	{
		glm::vec3 currentPos = TCB(k);
		mDistanceToNext[static_cast<int>(k - DISTANCE_RES)] += glm::distance(prevPos, currentPos);
		prevPos = currentPos;
	}
	for (int i = 1; i < static_cast<int>(mPositions.size()); i++)
	{
		for (int j = 0; j < i; j++)
		{
			mDistanceToPoint[i] += mDistanceToNext[j];
		}
	}
}
