#include "VertexBuffer.h"
#include "VertexBufferFile.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

namespace Destiny
{
	bool VertexBufferFile::ParseXml(const void* fileData, unsigned int length)
	{
		std::string str((const char*)fileData, length);
		rapidxml::xml_document<> doc;
		doc.parse<rapidxml::parse_declaration_node>((char*)str.c_str());

		rapidxml::xml_node<>* vertexBufferNode = doc.first_node("vertexBuffer");
		return true;

	}
}