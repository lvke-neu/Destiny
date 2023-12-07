#include "Model3D.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Destiny
{

	Model3D::Model3D()
	{

	}

	void Model3D::doLoad()
	{
		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFileFromMemory(blob->getData(), blob->getLength(), aiProcess_Triangulate | aiProcess_FlipUVs
					| aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices);

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
					loadFailed__();
					LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
					return;
				}
			}
			else
			{
				loadFailed__();
			}
		}
		else
		{
			loadFailed__();
		}
	}
}