#pragma once

#include "TileInfo.h"
#include <SFML/Graphics/Rect.hpp>	


struct Tile
{
	TileInfo* Properties;
	sf::FloatRect AABB;
	bool bWarp;
};
