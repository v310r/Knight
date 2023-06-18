#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <vector>


struct TileInfo;


struct CollisionManifold
{
	CollisionManifold(float area, TileInfo* tileInfo, const sf::FloatRect& bounds)
		: Area(area), TileInfo(tileInfo), TileBounds(bounds) {}


	float Area;
	TileInfo* TileInfo;
	sf::FloatRect TileBounds;
};

using Collisions = std::vector<CollisionManifold>;
