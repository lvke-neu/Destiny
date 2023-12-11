#include "Model3D.h"
#include "Model3DComponent.h"
#include "Node3D.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <DirectXMath.h>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Visual3DComponent.h"
#include "Graphics/Visual3D.h"
#include "Graphics/Material.h"

namespace Destiny
{
	using namespace DirectX;

	Model3D::Model3D(std::shared_ptr<Model3DComponent> model3DComponent) :
		m_model3DComponent(model3DComponent)
	{

	}

	Model3D::~Model3D()
	{
		for (const auto& v3dComponent : m_visual3DComponents)
		{
			if (m_model3DComponent && m_model3DComponent->get_node())
			{
				m_model3DComponent->get_node()->removeComponent(v3dComponent);
			}
		}
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
				std::string path((char*)blob->getData(), blob->getLength());
				size_t pos;

				char buffer[MAX_PATH];
				GetModuleFileNameA(NULL, buffer, sizeof(buffer));
				std::string exePath = buffer;
				pos = exePath.find("Destiny");
				if (pos != exePath.npos)
				{
					exePath = exePath.substr(0, pos + 7);
				}

				std::string tmpPath = path;
				pos = tmpPath.find("://");
				if (pos != tmpPath.npos)
				{
					tmpPath = tmpPath.substr(pos + 3);
				}

				Assimp::Importer importer;
				importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
				const aiScene* scene = importer.ReadFile(exePath + "\\assets\\" + tmpPath, aiProcess_ConvertToLeftHanded 
					| aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_SortByPType);

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
					loadFailed__();
					LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
					return;
				}

				m_visual3DComponents.resize(scene->mNumMeshes);

				std::shared_ptr<Blob> data = nullptr;
				struct VertexPosColor
				{
					XMFLOAT3 position;
					XMFLOAT3 normal;
					XMFLOAT2 texcoord;
				};

				D3D11_INPUT_ELEMENT_DESC inputElements[3] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				data.reset(new Blob(3 * sizeof(D3D11_INPUT_ELEMENT_DESC)));
				memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
				auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Basic_VS.cso");
				inputLayout->load(0);

				auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Basic_VS.cso");
				vertexShader->load(0);
				auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Basic_PS.cso");
				pixelShader->load(0);

				for (unsigned int i = 0; i < scene->mNumMeshes; i++)
				{

					m_visual3DComponents[i] = std::make_shared<Visual3DComponent>();

					aiMesh* mesh = scene->mMeshes[i];

					std::vector<VertexPosColor> vertices;
					vertices.resize(mesh->mNumVertices);

					for (unsigned int j = 0; j < mesh->mNumVertices; j++)
					{
						VertexPosColor vertex;

						vertex.position.x = mesh->mVertices[j].x;
						vertex.position.y = mesh->mVertices[j].y;
						vertex.position.z = mesh->mVertices[j].z;

						vertex.normal.x = mesh->mNormals[j].x;
						vertex.normal.y = mesh->mNormals[j].y;
						vertex.normal.z = mesh->mNormals[j].z;

						if (mesh->HasTextureCoords(0))
						{
							vertex.texcoord.x = mesh->mTextureCoords[0][j].x;
							vertex.texcoord.y = mesh->mTextureCoords[0][j].y;
						}

						vertices[j] = (vertex);
					}
					
					data.reset(new Blob(vertices.size() * sizeof(VertexPosColor)));
					memcpy_s(data->getData(), data->getLength(), vertices.data(), data->getLength());
					auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosColor), 0, data);
					vertexBuffer->load(0);
					m_visual3DComponents[i]->get_visual3D()->setVertexBuffer(vertexBuffer);

					std::vector<unsigned int> indices;

					for (unsigned int j = 0; j < mesh->mNumFaces; j++)
					{
						for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++)
						{
							indices.push_back(mesh->mFaces[j].mIndices[k]);
						}
					}
					data.reset(new Blob(sizeof(unsigned int) * indices.size()));
					memcpy_s(data->getData(), data->getLength(), indices.data(), data->getLength());
					auto indexBuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
					indexBuffer->load(0);
					m_visual3DComponents[i]->get_visual3D()->setIndexBuffer(indexBuffer);
				

					m_visual3DComponents[i]->get_visual3D()->setInputLayout(inputLayout);

					m_visual3DComponents[i]->get_visual3D()->setVertexShader(vertexShader);
					m_visual3DComponents[i]->get_visual3D()->setPixelShader(pixelShader);

					//material
					aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
					aiString str;
					aiColor4D color;

					aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
					aimaterial->GetTexture(aiTextureType_AMBIENT, 0, &str);
					m_visual3DComponents[i]->get_material()->set_ambientTexturePath(path.substr(0, path.rfind("/") + 1) + str.C_Str());
					m_visual3DComponents[i]->get_material()->set_ambientColor({ color.r, color.g, color.b, color.a });
					
					aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
					aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);
					m_visual3DComponents[i]->get_material()->set_diffuseTexturePath(path.substr(0, path.rfind("/") + 1) + str.C_Str());
					m_visual3DComponents[i]->get_material()->set_diffuseColor({ color.r, color.g, color.b, color.a });

					aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
					aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &str);
					m_visual3DComponents[i]->get_material()->set_specularTexturePath(path.substr(0, path.rfind("/") + 1) + str.C_Str());
					m_visual3DComponents[i]->get_material()->set_specularColor({ color.r, color.g, color.b, color.a });
					
					m_visual3DComponents[i]->get_material()->set_useColor(false);
					m_visual3DComponents[i]->get_material()->load();

					if (m_model3DComponent && m_model3DComponent->get_node())
					{
						auto meshNode = std::make_shared<Node3D>();
						meshNode->set_name(mesh->mName.C_Str());
						meshNode->addToParent(m_model3DComponent->get_node());
						meshNode->addComponent(m_visual3DComponents[i]);
						meshNode->set_transform3D(m_model3DComponent->get_node()->get_transform3D());
					}
				}

				loadSucceeded__();
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