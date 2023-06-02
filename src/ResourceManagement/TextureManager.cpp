#include "TextureManager.h"
#include <filesystem>


TextureManager::TextureManager() : ResourceManager("cfg/texturesPath.cfg")
{

}

sf::Texture* TextureManager::Load(const std::string& pathToFile)
{
	sf::Texture* texture = new sf::Texture();
	const std::filesystem::path absolutePath = std::filesystem::current_path() / pathToFile;
	if (!texture->loadFromFile(absolutePath.string()))
	{
		delete texture;
		texture = nullptr;
		std::cerr << "Failed to load texture: " << absolutePath.string() << std::endl;
	}
	return texture;
}
