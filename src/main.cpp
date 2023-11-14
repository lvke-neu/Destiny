//#include "GameApp.h"
//
//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
//    _In_ LPSTR cmdLine, _In_ int showCmd)
//{
//    // 这些参数不使用
//    UNREFERENCED_PARAMETER(prevInstance);
//    UNREFERENCED_PARAMETER(cmdLine);
//    UNREFERENCED_PARAMETER(showCmd);
//    // 允许在Debug版本进行运行时内存分配和泄漏检测
//#if defined(DEBUG) | defined(_DEBUG)
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//    GameApp theApp(hInstance, L"DirectX11 Initialization", 1280, 720);
//
//    if (!theApp.Init())
//        return 0;
//
//    return theApp.Run();
//}

//#include "Engine/Engine.h"
//
//int main()
//{
//	Destiny::Engine::GetInstance()->initialize();
//	Destiny::Engine::GetInstance()->run();
//	Destiny::Engine::GetInstance()->uninitialize();
//	
//	return 0;
//}

#include "Editor/Application.h"

int main()
{
	Application application;
	application.exec();

	return 0;
}

//#include "rapidxml.hpp"
//#include "rapidxml_print.hpp"
//#include "rapidxml_utils.hpp"
//#include "Engine/Base64.h"
//#include <iostream>
//int main()
//{
//	rapidxml::xml_document<> doc;
//
//	// 2.node_declaration
//	rapidxml::xml_node<>* declaration = doc.allocate_node(rapidxml::node_declaration);
//	declaration->append_attribute(doc.allocate_attribute("version", "1.0"));
//	declaration->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
//	doc.append_node(declaration);
//
//	//// 3.node_pi
//	//rapidxml::xml_node<>* dec = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
//	//doc.append_node(dec);
//
//	// 4.node_element
//	rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "root");
//	doc.append_node(root);
//	std::vector<int> arr{1,2,3,4,5,6,7,8,9,10};
//
//	char* data = new char[Destiny::Base64::EncodeBufferLength(40)];
//	Destiny::Base64::Encode(data, arr.data(), 40);
//
//	rapidxml::xml_node<>* students = doc.allocate_node(rapidxml::node_element, "students", data);
//	root->append_node(students);
//
//	//// 5.node_comment
//	//rapidxml::xml_node<>* comment = doc.allocate_node(rapidxml::node_comment, 0, "这是一个注释节点");
//	//root->append_node(comment);
//
//	//rapidxml::xml_node<>* students = doc.allocate_node(rapidxml::node_element, "students");
//
//	//// 6.node_data
//	//rapidxml::xml_node<>* one_student = doc.allocate_node(rapidxml::node_element, "student");
//	//rapidxml::xml_node<>* name = doc.allocate_node(rapidxml::node_data, "node_name", "11");
//	//one_student->append_node(name);
//	//students->append_node(one_student);
//
//	//// 7.node_element with value
//	//rapidxml::xml_node<>* two_student = doc.allocate_node(rapidxml::node_element, "student", "22");
//	//students->append_node(two_student);
//
//	//// 8.set attribute
//	//rapidxml::xml_node<>* three_student = doc.allocate_node(rapidxml::node_element, "student", "33");
//	//students->append_node(three_student);
//	//three_student->append_attribute(doc.allocate_attribute("course", doc.allocate_string(buf)));
//	//three_student->append_attribute(doc.allocate_attribute("score", "98"));
//
//	//// 9.node_element without value
//	//rapidxml::xml_node<>* four_student = doc.allocate_node(rapidxml::node_element, "student");
//	//students->append_node(four_student);
//
//	//// 10.node_cdata
//	//rapidxml::xml_node<>* five_student = doc.allocate_node(rapidxml::node_cdata, "student", "55");
//	//students->append_node(five_student);
//
//	//// 11.node_cdata
//	//rapidxml::xml_node<>* six_student = doc.allocate_node(rapidxml::node_pi, "student", "66");
//	//students->append_node(six_student);
//
//	//// 12.node_cdata
//	//rapidxml::xml_node<>* seven_student = doc.allocate_node(rapidxml::node_doctype, "student", "77");
//	//students->append_node(seven_student);
//	//root->append_node(students);
//
//	// 13.输出DOM到命令行
//	//std::cout << doc;
//	char* buf = new char[2048];
//	char* end = rapidxml::print(buf, doc, 0);
//	*end = 0;
//	 std::string text;
//rapidxml::print(std::back_inserter(text), doc, 0);
//	int i = 0;
//	i++;
//}

//#include "Editor/Application.h"
//#include "Engine/Engine.h"
//#include "Engine/Blob.h"
//#include "Engine/BlobLoader.h"
//#include "Engine/BlobHolder.h"
//#include "Engine/BlobLoaderManager.h"
//#include "Engine/Detail/ResourceBlobLoader.h"
//#include "Graphics/VertexBuffer.h"
//#include "Graphics/IndexBuffer.h"
//#include "Graphics/Mesh.h"
//#include "Graphics/VertexShader.h"
//#include "Graphics/PixelShader.h"
//#include "Graphics/InputLayout.h"
//#include "Graphics/GraphicsSystem.h"
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include <fstream>
//
//int main()
//{
//	using namespace Destiny;
//
//	Application application;
//
//	
//
//	//auto blobloader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("assets://test.vtb");
//	//auto blobholder = blobloader->createBlobHolder("assets://test.vtb");
//
//	//auto vertexbuffer = Engine::GetInstance()->getGraphicsAssetLoader()->getVertexBufferLoader()->createAsset(blobholder);
//	//vertexbuffer->load();
//	using namespace DirectX;
//	struct VertexPosColor
//	{
//		XMFLOAT3 pos;
//		XMFLOAT3 normal;
//		XMFLOAT2 tex;
//	};
//
//	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	VertexPosColor vertices[24];
//	vertices[0].pos = XMFLOAT3(1, 2, 3);
//	vertices[1].pos = XMFLOAT3(4, 5, 6);
//	vertices[2].pos = XMFLOAT3(7, 8, 9);
//	vertices[3].pos = XMFLOAT3(1, -1, 1);
//	vertices[4].pos = XMFLOAT3(-1, -1, 1);
//	vertices[5].pos = XMFLOAT3(-1, 1, 1);
//	vertices[6].pos = XMFLOAT3(-1, 1, -1);
//	vertices[7].pos = XMFLOAT3(-1, -1, -1);
//	vertices[8].pos = XMFLOAT3(-1, 1, -1);
//	vertices[9].pos = XMFLOAT3(-1, 1, 1);
//	vertices[10].pos = XMFLOAT3(1, 1, 1);
//	vertices[11].pos = XMFLOAT3(1, 1, -1);
//	vertices[12].pos = XMFLOAT3(1, -1, -1);
//	vertices[13].pos = XMFLOAT3(1, -1, 1);
//	vertices[14].pos = XMFLOAT3(-1, -1, 1);
//	vertices[15].pos = XMFLOAT3(-1, -1, -1);
//	vertices[16].pos = XMFLOAT3(1, -1, 1);
//	vertices[17].pos = XMFLOAT3(1, 1, 1);
//	vertices[18].pos = XMFLOAT3(-1, 1, 1);
//	vertices[19].pos = XMFLOAT3(-1, -1, 1);
//	vertices[20].pos = XMFLOAT3(-1, -1, -1);
//	vertices[21].pos = XMFLOAT3(-1, 1, -1);
//	vertices[22].pos = XMFLOAT3(1, 1, -1);
//	vertices[23].pos = XMFLOAT3(250, 222, 221);
//	for (UINT i = 0; i < 4; ++i)
//	{
//		vertices[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
//		vertices[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
//		vertices[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
//		vertices[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
//		vertices[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
//		vertices[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
//	}
//	for (UINT i = 0; i < 6; ++i)
//	{
//		vertices[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
//		vertices[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
//		vertices[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
//		vertices[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
//	}
//
//	unsigned int indices[36] = {
//			0, 1, 2, 2, 3, 0,		// 右面(+X面)
//			4, 5, 6, 6, 7, 4,		// 左面(-X面)
//			8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
//			12, 13, 14, 14, 15, 12,	// 底面(-Y面)
//			16, 17, 18, 18, 19, 16, // 背面(+Z面)
//			20, 21, 22, 22, 23, 20	// 正面(-Z面)
//	};
//
//	std::shared_ptr<Blob> vertexData = std::make_shared<Blob>(24 * sizeof(VertexPosColor));
//	memcpy_s(vertexData->getData(), vertexData->getLength(), vertices, vertexData->getLength());
//	auto vertexbuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosColor), 0, vertexData);
//
//	vertexbuffer->load();
//
//	std::shared_ptr<Blob> data = std::make_shared<Blob>(36 * sizeof(unsigned int));
//	memcpy_s(data->getData(), 36 * sizeof(unsigned int), indices, 36 * sizeof(unsigned int));
//	auto indexbuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
//	indexbuffer->load();
//
//	auto mesh = Engine::GetInstance()->getGraphicsSystem()->createMesh(vertexbuffer, nullptr);
//	mesh->load(0);
//	//auto vs = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Phong_VS.cso");
//	//vs->load();
//
//	//auto ps = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Phong_PS.cso");
//	//ps->load();
//
//	//std::shared_ptr<Blob> data = std::make_shared<Blob>(3 * sizeof(D3D11_INPUT_ELEMENT_DESC));
//	//memcpy_s(data->getData(), data->getLength(), inputElements.data(), data->getLength());
//	//auto inputlayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Phong_VS.cso");
//	//inputlayout->load();
//
//	application.exec();
//
//	return 0;
//}