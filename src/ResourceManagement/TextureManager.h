#pragma once

#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>


class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager();

private:
	sf::Texture* Load(const std::string& path);
};

