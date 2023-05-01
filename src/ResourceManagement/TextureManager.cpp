#include "TextureManager.h"

TextureManager::TextureManager() : ResourceManager("cfg/textures.cfg")
{

}

sf::Texture* TextureManager::Load(const std::string& path)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile((std::filesystem::current_path() / path).string()))
	{
		delete texture;
		texture = nullptr;
		std::cerr << "Failed to load texture: " << path << std::endl;
	}
	return texture;
}
