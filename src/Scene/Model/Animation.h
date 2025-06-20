#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>

namespace Destiny
{
	struct BoneInfo
	{
		DirectX::XMMATRIX offsetMatrix;
		DirectX::XMMATRIX finalTransformation;

		BoneInfo(const DirectX::XMMATRIX& Offset)
		{
			offsetMatrix = Offset;
			DirectX::XMMATRIX M;
			M.r[0] = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			M.r[1] = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			M.r[2] = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			M.r[3] = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			finalTransformation = M;
		}
	};


	class Skeleton
	{
	public:
		int GetBoneId(const std::string& BoneName)
		{
			int BoneIndex = 0;
			
			if (boneNameToIndexMap.find(BoneName) == boneNameToIndexMap.end()) {
				// Allocate an index for a new bone
				BoneIndex = (int)boneNameToIndexMap.size();
				boneNameToIndexMap[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = boneNameToIndexMap[BoneName];
			}

			return BoneIndex;
		}
	public:
		std::map<std::string, unsigned int> boneNameToIndexMap;
		std::vector<BoneInfo> boneInfo;
	};


	struct Animation
	{
		std::string name;     
		double duration;      
		double ticksPerSecond;

		struct Channel
		{
			std::string nodeName; 
			std::vector<DirectX::XMFLOAT3> positionKeys;
			std::vector<float> positionTimes;      

			std::vector<DirectX::XMFLOAT4> rotationKeys;
			std::vector<float> rotationTimes;      

			std::vector<DirectX::XMFLOAT3> scalingKeys;
			std::vector<float> scalingTimes;   
		};

		std::vector<std::shared_ptr<Channel>> channels; 
	};

	class Node;
	class Animator
	{
	public:
		Animator();
	public:
		std::string get_animation();
		void set_animation(std::string animation);
		unsigned int get_animationIndex();
		void set_animationIndex(unsigned int animationIndex);
		unsigned int get_animationCount();
		void set_animationCount(unsigned int animationCount);
		float get_animationSpeed();
		void set_animationSpeed(float animationSpeed);
	public:
		void calcuFinalTransform(float TimeInSeconds);
		void readNodeHierarchy(float TimeInSeconds, std::shared_ptr<Node> pNode, const DirectX::XMMATRIX& ParentTransform);
	public:
		DirectX::XMMATRIX globalInverseTransform;
		long long startTimeMillis = 0;
	public:
		std::shared_ptr<Skeleton> m_skeleton;
		std::vector<std::shared_ptr<Animation>> m_animations;

		std::shared_ptr<Node> m_node;

		unsigned int m_animationIndex;

		float m_animationSpeed;
	};
}