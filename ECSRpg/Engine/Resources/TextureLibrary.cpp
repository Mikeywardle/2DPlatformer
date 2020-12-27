#include "TextureLibrary.h"
#include <Utils/XMLReader.h>

std::string TextureLibrary::TEXTURE_PATH = "Textures";

TextureLibrary::TextureLibrary()
{
    LoadFileMetaData();
}

void TextureLibrary::LoadTexture(std::string name, std::string filepath, bool alpha)
{
    std::string texturePath = "Resources\\" + TEXTURE_PATH + "\\" + filepath;
    resourceTable[name] = ImportTextureFromFile(texturePath.c_str(), alpha);
}

Texture2D* TextureLibrary::GetTexture(std::string name)
{
    if (resourceTable.find(name) == resourceTable.end())
    {
        TextureImportData textureData = FileLookupTable[name];
        LoadTexture(name ,textureData.path, textureData.alpha);
    }        

    return &resourceTable[name];
}

void TextureLibrary::LoadFileMetaData()
{
    std::string texturesPath = "Resources\\" + TEXTURE_PATH + "\\_Textures.xml";
    xml_document* metaDoc = XMLReader::parseXml(texturesPath);

    for (xml_node textureNode = metaDoc->child("Textures").child("Texture"); textureNode; textureNode = textureNode.next_sibling())
    {
        std::string filepath = textureNode.child("file").first_child().value();
        std::string name = textureNode.child("name").first_child().value();
        std::string alpha = textureNode.child("alpha").first_child().value();

        FileLookupTable[name] = TextureImportData(filepath, alpha=="true");
    }

    delete metaDoc;
}

TextureLibrary::TextureImportData::TextureImportData(std::string path, bool alpha)
{
    this->path = path;
    this->alpha = alpha;
}
