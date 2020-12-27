#pragma once

#include <pugixml/pugixml.hpp>

#include <string>

using namespace pugi;

class XMLReader
{
public:
	static xml_document* parseXml(std::string filePath);
};