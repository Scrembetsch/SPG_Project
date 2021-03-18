#pragma once

#include <glm/glm.hpp>

#include <algorithm>
#include <vector>

#include "KdNode.h"
#include "Ray.h"
#include "Timing.h"
#include "../Primitives/BoundingBox.h"
#include "../Primitives/Triangle.h"

class KdTree
{
public:

	KdTree()
		: mLastHit(nullptr)
		, mHitDistance(0)
		, mRootNode(nullptr)
	{
	}

	~KdTree()
	{
	}

	bool CheckIntersect(const Ray& ray)
	{
		Timing::clear();
		mLastHit = nullptr;
		float hitT = 999999;

		Timing::getInstance()->startComputation();
		VisitNode(mRootNode, ray, &mLastHit, hitT);
		Timing::getInstance()->stopComputation();
		Timing::getInstance()->print();
		
		mLastHit = mLastHit;
		ResetTriangles(mRootNode);
		if (mLastHit != nullptr)
		{
			mLastHit->mIsHit = true;
			mHitDistance = hitT;
			mLastHitPosition = ray.mOrigin + ray.mDirection * hitT;
			return true;
		}
		return false;
	}

	void BuildTree(std::vector<Triangle*>* triangles, int maxVerticesInLeaf)
	{
		std::cout << "Start kd-tree build..." << std::endl;
		Timing::getInstance()->startSetup();
		mRootNode = new KdNode();
		std::vector<KdVertex*>* vertices = new std::vector<KdVertex*>();
		BoundingBox* bb = new BoundingBox();

		for (auto tri : *triangles)
		{
			vertices->push_back(new KdVertex(tri->mVertices[0], tri));
			bb->AddVertex(tri->mVertices[0], false);
			vertices->push_back(new KdVertex(tri->mVertices[1], tri));
			bb->AddVertex(tri->mVertices[1], false);
			vertices->push_back(new KdVertex(tri->mVertices[2], tri));
			bb->AddVertex(tri->mVertices[2], false);
		}
		bb->UpdateLineArray();
		mRootNode->mBoundingBox = bb;
		BuildTree(mRootNode, vertices, maxVerticesInLeaf);
		Timing::getInstance()->stopSetup();
		Timing::getInstance()->print(true);
	}
	
	void DrawTree()
	{
		mRootNode->Draw();
	}

	KdNode* mRootNode;

	Triangle* mLastHit;
    glm::vec3 mLastHitPosition;
    float mHitDistance;

protected:
	void BuildTree(KdNode* node, std::vector<KdVertex*>* vertices, int maxVerticesInLeaf)
	{
		if (vertices->size() > maxVerticesInLeaf)
		{
			auto mid = vertices->begin() + (std::distance(vertices->begin(), vertices->end()) / 2);

			if (vertices->size() % 2 == 0)
			{
				mid -= 1;
			}

			node->mAxis = node->mBoundingBox->LargestAxis();

			std::vector<KdVertex*>* leftVertices = new std::vector<KdVertex*>();
			BoundingBox* leftBb = new BoundingBox();
			node->mNode1 = new KdNode();
			std::vector<KdVertex*>* rightVertices = new std::vector<KdVertex*>();
			BoundingBox* rightBb = new BoundingBox();
			node->mNode2 = new KdNode();
			
			switch (node->mAxis)
			{
				case Node::Axis::X:
					std::nth_element(vertices->begin(), mid, vertices->end(), KdVertex::CompareX);
					node->mPoint = *mid;
					for (auto vertex : *vertices)
					{
						if (vertex->mPos.x <= node->mPoint->mPos.x)
						{
							leftVertices->push_back(vertex);
							leftBb->AddVertex(vertex->mPos, false);
						}
						else
						{
							rightVertices->push_back(vertex);
							rightBb->AddVertex(vertex->mPos, false);
						}
					}
					break;

				case Node::Axis::Y:
					std::nth_element(vertices->begin(), mid, vertices->end(), KdVertex::CompareY);
					node->mPoint = *mid;
					for (auto vertex : *vertices)
					{
						if (vertex->mPos.y <= node->mPoint->mPos.y)
						{
							leftVertices->push_back(vertex);
							leftBb->AddVertex(vertex->mPos, false);
						}
						else
						{
							rightVertices->push_back(vertex);
							rightBb->AddVertex(vertex->mPos, false);
						}
					}
					break;

				case Node::Axis::Z:
					std::nth_element(vertices->begin(), mid, vertices->end(), KdVertex::CompareZ);
					node->mPoint = *mid;
					for (auto vertex : *vertices)
					{
						if (vertex->mPos.z <= node->mPoint->mPos.z)
						{
							leftVertices->push_back(vertex);
							leftBb->AddVertex(vertex->mPos, false);
						}
						else
						{
							rightVertices->push_back(vertex);
							rightBb->AddVertex(vertex->mPos, false);
						}
					}
					break;

				default:
					node->mNodePoints = vertices;
					break;
			}
			leftBb->UpdateLineArray();
			rightBb->UpdateLineArray();
			node->mNode1->mBoundingBox = leftBb;
			node->mNode2->mBoundingBox = rightBb;
			BuildTree(node->mNode1, leftVertices, maxVerticesInLeaf);
			BuildTree(node->mNode2, rightVertices, maxVerticesInLeaf);
		}
		else
		{
			node->mNodePoints = vertices;
		}
	}

	void VisitNode(KdNode* node, const Ray& ray, Triangle** closestHit, float& closestT)
	{
		if (node == nullptr)
		{
			return;
		}

		float minT = ray.mMinDistance;
		if (node->mBoundingBox->Intersect(ray))
		{
			if (node->mAxis == Node::Axis::INVALID)
			{
				for (auto vert : *node->mNodePoints)
				{
					if (vert->mParentTriangle->mHitChecked == false)
					{
						float t = 0;
						if (vert->mParentTriangle->Intersects(ray, t))
						{
							if (t < closestT)
							{
								*closestHit = vert->mParentTriangle;
								closestT = t;
							}
						}
					}
				}
			}
			else
			{
				VisitNode(node->mNode1, ray, closestHit, closestT);
				VisitNode(node->mNode2, ray, closestHit, closestT);
			}
		}
	}

	void ResetTriangles(KdNode* node)
	{
		if (node->mAxis == Node::Axis::INVALID)
		{
			for (auto vertex : *node->mNodePoints)
			{
				vertex->mParentTriangle->mIsHit = false;
				vertex->mParentTriangle->mHitChecked = false;
			}
		}
		else
		{
			ResetTriangles(node->mNode1);
			ResetTriangles(node->mNode2);
		}
	}

	std::vector<KdVertex*> mVertices;
};