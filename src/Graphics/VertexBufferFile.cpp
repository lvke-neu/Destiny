#include "VertexBuffer.h"
#include "VertexBufferFile.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "GraphicsFormatUtility.h"
#include "Engine/Base64.h"
#include "Engine/Blob.h"

namespace Destiny
{
	bool VertexBufferFile::ParseXml(const void* fileData, unsigned int length)
	{
		std::string str((const char*)fileData, length);
		rapidxml::xml_document<> doc;
		doc.parse<rapidxml::parse_declaration_node>((char*)str.c_str());

		rapidxml::xml_node<>* vertexBufferNode = doc.first_node("VertexBuffer");
		
		
		return true;

	}

	std::shared_ptr<Blob> VertexBufferFile::BuildVertexBufferFile(D3D11_INPUT_ELEMENT_DESC* inputElements, unsigned int inputElementsLength, unsigned int stride, unsigned int offset, const void* vertexData, unsigned int vertexDataLength)
	{
		rapidxml::xml_document<> doc;

		rapidxml::xml_node<>* declaration = doc.allocate_node(rapidxml::node_declaration);
		declaration->append_attribute(doc.allocate_attribute("version", "1.0"));
		declaration->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
		doc.append_node(declaration);

		rapidxml::xml_node<>* VertexBufferNode = doc.allocate_node(rapidxml::node_element, "VertexBuffer");
		doc.append_node(VertexBufferNode);

		rapidxml::xml_node<>* InputElementsNode = doc.allocate_node(rapidxml::node_element, "InputElements");
		VertexBufferNode->append_node(InputElementsNode);
		InputElementsNode->append_attribute(doc.allocate_attribute("ArraySize", doc.allocate_string(std::to_string(inputElementsLength).c_str())));
		for (unsigned int i = 0; i < inputElementsLength; ++i)
		{
			rapidxml::xml_node<>* InputElementNode = doc.allocate_node(rapidxml::node_element, doc.allocate_string(std::to_string(i).c_str()));
			InputElementsNode->append_node(InputElementNode);

			rapidxml::xml_node<>* SemanticNameNode = doc.allocate_node(rapidxml::node_element, "SemanticName", doc.allocate_string(inputElements[i].SemanticName));
			InputElementNode->append_node(SemanticNameNode);

			rapidxml::xml_node<>* SemanticIndexNode = doc.allocate_node(rapidxml::node_element, "SemanticIndex", doc.allocate_string(std::to_string(inputElements[i].SemanticIndex).c_str()));
			InputElementNode->append_node(SemanticIndexNode);
			
			rapidxml::xml_node<>* FormatNameNode = doc.allocate_node(rapidxml::node_element, "Format", doc.allocate_string(GraphicsFormatUtility::DXGI_FORMAT_TO_STRING(inputElements[i].Format).c_str()));
			InputElementNode->append_node(FormatNameNode);

			rapidxml::xml_node<>* InputSlotNameNode = doc.allocate_node(rapidxml::node_element, "InputSlot", doc.allocate_string(std::to_string(inputElements[i].InputSlot).c_str()));
			InputElementNode->append_node(InputSlotNameNode);

			rapidxml::xml_node<>* AlignedByteOffsetNameNode = doc.allocate_node(rapidxml::node_element, "AlignedByteOffset", doc.allocate_string(std::to_string(inputElements[i].AlignedByteOffset).c_str()));
			InputElementNode->append_node(AlignedByteOffsetNameNode);

			rapidxml::xml_node<>* InputSlotClassNameNode = doc.allocate_node(rapidxml::node_element, "InputSlotClass", doc.allocate_string(GraphicsFormatUtility::D3D11_INPUT_CLASSIFICATION_TO_STRING(inputElements[i].InputSlotClass).c_str()));
			InputElementNode->append_node(InputSlotClassNameNode);
	
		}

		rapidxml::xml_node<>* StrideNode = doc.allocate_node(rapidxml::node_element, "Stride", doc.allocate_string(std::to_string(stride).c_str()));
		VertexBufferNode->append_node(StrideNode);

		rapidxml::xml_node<>* OffsetNode = doc.allocate_node(rapidxml::node_element, "Offset", doc.allocate_string(std::to_string(offset).c_str()));
		VertexBufferNode->append_node(OffsetNode);

		size_t vertexDataEncodeLength = Base64::EncodeBufferLength(vertexDataLength);
		char* buf = new char[vertexDataEncodeLength];
		Base64::Encode(buf, vertexData, vertexDataLength);
		rapidxml::xml_node<>* VertexDataLengthNode = doc.allocate_node(rapidxml::node_element, "VertexDataLength", doc.allocate_string(std::to_string(vertexDataEncodeLength).c_str()));
		VertexBufferNode->append_node(VertexDataLengthNode);
		rapidxml::xml_node<>* VertexDataNode = doc.allocate_node(rapidxml::node_element, "VertexData", doc.allocate_string(buf));
		VertexBufferNode->append_node(VertexDataNode);
		delete[] buf;
		
		std::string text;
		rapidxml::print(std::back_inserter(text), doc, 0);

		std::shared_ptr<Blob> blob = std::make_shared<Blob>(text.size());
		memcpy_s(blob->getData(), blob->getLength(), text.data(), blob->getLength());

		return blob;
	}
}