#pragma once

#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager();

	sf::Texture* Load(const std::string& pathToFile);
};

