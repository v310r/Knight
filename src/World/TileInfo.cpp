#include "TileInfo.h"
#include "../SharedContext/SharedContext.h"
#include "../ResourceManagement/TextureManager.h"
#include <SFML/Graphics/Rect.hpp>
#include "../Utilities/TileHelper.h"


TileInfo::TileInfo(SharedContext* context, const std::string& textureName, TileID id) : Context(context)
{
	TextureManager* textureManager = Context->GetTextureManager();
	if (TextureName.empty())
	{
		ID = id;
		return;
	}

	if (textureManager->RequireResource(textureName))
	{
		return;
	}

	TextureName = textureName;
	ID = id;
	if (const sf::Texture* const texture = textureManager->GetResource(TextureName))
	{
		Sprite.setTexture(*texture);
	}

	const int LeftX = ID % (TileSheet::Width / TileSheet::TileSize) * TileSheet::TileSize;
	const int LeftY = ID / (TileSheet::Height / TileSheet::TileSize) * TileSheet::TileSize;
	const int SizeX = TileSheet::TileSize;
	const int SizeY = TileSheet::TileSize;
	const sf::IntRect tileBoundaries(LeftX, LeftY, SizeX, SizeY);
	Sprite.setTextureRect(tileBoundaries);
}

TileInfo::~TileInfo()
{
	if (TextureName.empty())
	{
		return;
	}

	if (TextureManager* textureManager = Context->GetTextureManager())
	{
		textureManager->ReleaseResource(TextureName);
	}
	
}
