#include "Animation.h"
#include "Engine/Node.h"
#include <assimp/quaternion.h>
#include <assimp/matrix3x3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Destiny
{
	Animator::Animator() :
		m_skeleton(std::make_shared<Skeleton>()),
		m_animationIndex(-1)
	{

	}

	unsigned int FindScaling(float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->scalingKeys.size() - 1; i++) {
			float t = pNodeAnim->scalingTimes[i + 1];
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}

	void CalcInterpolatedScaling(DirectX::XMFLOAT3& Out, float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->scalingKeys.size() == 1) {
			Out = pNodeAnim->scalingKeys[0];
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
		unsigned int NextScalingIndex = ScalingIndex + 1;

		float t1 = (float)pNodeAnim->scalingTimes[ScalingIndex];
		float t2 = (float)pNodeAnim->scalingTimes[NextScalingIndex];
		float DeltaTime = t2 - t1;
		float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;

		auto Start = pNodeAnim->scalingKeys[ScalingIndex];
		auto End = pNodeAnim->scalingKeys[NextScalingIndex];
		DirectX::XMFLOAT3 Delta = { End.x - Start.x, End.y - Start.y, End.z - Start.z };

		Out = { Start.x + Factor * Delta.x, Start.y + Factor * Delta.y, Start.z + Factor * Delta.z };
	}

	unsigned int FindRotation(float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		for (unsigned i = 0; i < pNodeAnim->rotationKeys.size() - 1; i++) {
			float t = (float)pNodeAnim->rotationTimes[i + 1];
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->rotationKeys.size() == 1) {
			Out =
			{
				pNodeAnim->rotationKeys[0].w,
				pNodeAnim->rotationKeys[0].x,
				pNodeAnim->rotationKeys[0].y,
				pNodeAnim->rotationKeys[0].z
			};
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
		unsigned NextRotationIndex = RotationIndex + 1;

		float t1 = (float)pNodeAnim->rotationTimes[RotationIndex];
		float t2 = (float)pNodeAnim->rotationTimes[NextRotationIndex];
		float DeltaTime = t2 - t1;
		float Factor = (AnimationTimeTicks - t1) / DeltaTime;

		aiQuaternion StartRotationQ;
		StartRotationQ.x = pNodeAnim->rotationKeys[RotationIndex].x;
		StartRotationQ.y = pNodeAnim->rotationKeys[RotationIndex].y;
		StartRotationQ.z = pNodeAnim->rotationKeys[RotationIndex].z;
		StartRotationQ.w = pNodeAnim->rotationKeys[RotationIndex].w;

		aiQuaternion EndRotationQ;
		EndRotationQ.x = pNodeAnim->rotationKeys[NextRotationIndex].x;
		EndRotationQ.y = pNodeAnim->rotationKeys[NextRotationIndex].y;
		EndRotationQ.z = pNodeAnim->rotationKeys[NextRotationIndex].z;
		EndRotationQ.w = pNodeAnim->rotationKeys[NextRotationIndex].w;

		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out.Normalize();
	}

	unsigned int FindPosition(float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->positionKeys.size() - 1; i++) {
			float t = (float)pNodeAnim->positionTimes[i + 1];
			if (AnimationTimeTicks < t) {
				return i;
			}
		}

		return 0;
	}

	void CalcInterpolatedPosition(DirectX::XMFLOAT3& Out, float AnimationTimeTicks, std::shared_ptr<Animation::Channel> pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->positionKeys.size() == 1) {
			Out = pNodeAnim->positionKeys[0];
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
		unsigned int NextPositionIndex = PositionIndex + 1;

		float t1 = (float)pNodeAnim->positionTimes[PositionIndex];
		float t2 = (float)pNodeAnim->positionTimes[NextPositionIndex];
		float DeltaTime = t2 - t1;
		float Factor = (AnimationTimeTicks - t1) / DeltaTime;

		auto Start = pNodeAnim->positionKeys[PositionIndex];
		auto End = pNodeAnim->positionKeys[NextPositionIndex];

		DirectX::XMFLOAT3 Delta = { End.x - Start.x,  End.y - Start.y, End.z - Start.z };
		Out = { Start.x + Factor * Delta.x, Start.y + Factor * Delta.y, Start.z + Factor * Delta.z };
	}

	void Animator::readNodeHierarchy(float TimeInSeconds, std::shared_ptr<Node> pNode, const DirectX::XMMATRIX& ParentTransform)
	{
		if (!pNode)
		{
			return;
		}

		std::string NodeName(pNode->get_name());

		auto pAnimation = m_animations[m_animationIndex];

		auto NodeTransformation(pNode->getWorldMatrix());


		std::shared_ptr<Animation::Channel> pNodeAnim = nullptr;

		for (const auto& channel : pAnimation->channels)
		{
			if (channel && NodeName == channel->nodeName)
			{
				pNodeAnim = channel;
				break;
			}
		}


		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			DirectX::XMFLOAT3 Scaling;
			CalcInterpolatedScaling(Scaling, TimeInSeconds, pNodeAnim);
			DirectX::XMMATRIX ScalingM =
			{ Scaling.x, 0.0f,   0.0f,   0.0f,
			  0.0f,   Scaling.y, 0.0f, 0.0f,
			  0.0f,   0.0f, Scaling.z, 0.0f,
			  0.0f,   0.0f, 0.0f, 1.0f
			};

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, TimeInSeconds, pNodeAnim);
			aiMatrix3x3 mat3x3 = RotationQ.GetMatrix();
			DirectX::XMMATRIX RotationM =
			{
				mat3x3.a1, mat3x3.a2, mat3x3.a3, 0.0f,
				mat3x3.b1, mat3x3.b2, mat3x3.b3, 0.0f,
				mat3x3.c1, mat3x3.c2, mat3x3.c3, 0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			};

			// Interpolate translation and generate translation transformation matrix
			DirectX::XMFLOAT3 Translation;
			CalcInterpolatedPosition(Translation, TimeInSeconds, pNodeAnim);
			DirectX::XMMATRIX TranslationM;
			TranslationM =
			{
				1.0f,  0.0f, 0.0f,  Translation.x,
				0.0f,  1.0f, 0.0f,  Translation.y,
				0.0f,  0.0f, 1.0f,  Translation.z,
				0.0f,  0.0f, 0.0f,  1.0f
			};


			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
			//NodeTransformation =  ScalingM * RotationM * TranslationM;
			//NodeTransformation = ScalingM * RotationM * TranslationM;
		}

		//notice glm matrix multiplication : need Swap two matrix's positions  !!!!!!!
		auto GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_skeleton->boneNameToIndexMap.find(NodeName) != m_skeleton->boneNameToIndexMap.end()) {
			unsigned int BoneIndex = m_skeleton->boneNameToIndexMap[NodeName];

			//notice glm matrix multiplication : need Swap two matrix's positions !!!!!!!!!!
			m_skeleton->boneInfo[BoneIndex].finalTransformation = globalInverseTransform * GlobalTransformation * m_skeleton->boneInfo[BoneIndex].offsetMatrix;
		}

		for (const auto& child : pNode->getChilds())
		{
			readNodeHierarchy(TimeInSeconds, child, GlobalTransformation);
		}
	}

	void Animator::calcuFinalTransform(float TimeInSeconds)
	{
		if (!m_animations.size())
		{
			return;
		}

		float TicksPerSecond = (float)(m_animations[m_animationIndex]->ticksPerSecond != 0 ? m_animations[m_animationIndex]->ticksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTimeTicks = fmod(TimeInTicks, (float)m_animations[m_animationIndex]->duration);

		readNodeHierarchy(AnimationTimeTicks, m_node, DirectX::XMMatrixIdentity());
	}


	std::string Animator::get_animation()
	{
		if (m_animationIndex >= 0 && m_animationIndex < m_animations.size())
		{
			return m_animations[m_animationIndex]->name;
		}

		return "";
	}

	void Animator::set_animation(std::string animation)
	{

	}

	unsigned int Animator::get_animationIndex()
	{
		return m_animationIndex;
	}

	void Animator::set_animationIndex(unsigned int animationIndex)
	{
		if (animationIndex >= 0 && animationIndex < m_animations.size())
		{
			m_animationIndex = animationIndex;
		}
	}

	unsigned int Animator::get_animationCount()
	{
		return (unsigned int)m_animations.size();
	}

	void Animator::set_animationCount(unsigned int animationCount)
	{

	}
}