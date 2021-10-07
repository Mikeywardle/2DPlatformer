#pragma once

#include "ResourceLibrary.h"
#include <Resources/Texture2D.h>

class TextureLibrary : public ResourceLibrary<Texture2D>
{
public :

	TextureLibrary();

	void LoadTexture(std::string name, std::string filepath, bool alpha);
	Texture2D* GetTexture(std::string name);

	virtual void ClearAssets() override;

private:
	static std::string TEXTURE_PATH;

	struct TextureImportData
	{
		TextureImportData() = default;
		TextureImportData(std::string path, bool alpha);

		std::string path;
		bool alpha;
	};

	std::unordered_map<std::string, TextureImportData> FileLookupTable;

	void LoadFileMetaData();
};
