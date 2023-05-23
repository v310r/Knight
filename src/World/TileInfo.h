#pragma once

#include "../Utilities/TileHelper.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>


class SharedContext;

struct TileInfo
{
	TileInfo(SharedContext* context, const std::string& textureName = "TileSheet", TileID id = 0);

	~TileInfo();


	sf::Sprite Sprite;

	TileID ID = 0;
	std::string Name;
	std::string TextureName;

	sf::Vector2f Friction;

	bool bDeadly = false;

	SharedContext* Context;
};
