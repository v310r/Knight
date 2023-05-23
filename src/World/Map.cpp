#include "Map.h"

#include "../Window/Window.h"
#include "../ResourceManagement/TextureManager.h"
#include "../States/StateManager.h"
#include <cmath>


Map::Map(SharedContext* context, BaseState* currentState) 
	: m_Context(context), m_DefaultTile(context), m_CurrentState(currentState)
{
	m_Context->SetMap(this);
	LoadTiles("cfg/Tiles.cfg");
}

Map::~Map()
{
	PurgeEverything();
	PurgeTileSet();
	m_Context->SetMap(nullptr);
}

Tile* Map::GetTile(unsigned int x, unsigned int y)
{
	auto iter = m_Tiles.find(ConvertCoords(x, y));

	return (iter != m_Tiles.end()) ? iter->second : nullptr;
}

TileInfo* Map::GetDefaultTile()
{
	return nullptr;
}

float Map::GetGravity() const
{
	return 0.0f;
}

unsigned int Map::GetTileSize() const
{
	return 0u;
}

const sf::Vector2f& Map::GetMapSize() const
{
	return {};
}

const sf::Vector2f& Map::GetPlayerStart() const
{
	return {};
}

void Map::LoadMap(const std::string& path)
{
	std::ifstream file;
	file.open(std::filesystem::current_path() / path);
	if (!file.is_open())
	{
		std::cerr << "Failed to load a map file: " << path << std::endl;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TILE")
		{
			int tileID = 0;
			keystream >> tileID;
			if (tileID < 0)
			{
				std::cerr << "Bad tile id: " << tileID << std::endl;
				continue;
			}

			auto iter = m_TileSet.find(tileID);
			if (iter == m_TileSet.end())
			{
				std::cerr << "Tile id(" << tileID << ") was not found in tileset." << std::endl;
				continue;
			}

			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_MaxSize.x || tileCoords.y > m_MaxSize.y)
			{
				std::cerr << "Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}

			Tile* tile = new Tile();
			tile->Properties = iter->second;
			if (m_Tiles.emplace(ConvertCoords(tileCoords.x, tileCoords.y), tile).second)
			{
				std::cerr << "Duplicate tile at: (" << tileCoords.x << ", " << tileCoords.y << ")" << std::endl;
				delete tile;
				continue;
			}

			std::string warp;
			keystream >> warp;
			tile->bWarp = false;
			if (warp == "WARP")
			{
				tile->bWarp = true;
			}
		}
		else if (type == "BACKGROUND")
		{
			if (!m_BackgroundTextureName.empty())
			{
				continue;
			}

			keystream >> m_BackgroundTextureName;
			if (!m_Context->GetTextureManager()->RequireResource(m_BackgroundTextureName))
			{
				m_BackgroundTextureName = "";
				continue;
			}

			const sf::Texture* const texture = m_Context->GetTextureManager()->GetResource(m_BackgroundTextureName);
			m_Background.setTexture(*texture);

			const sf::Vector2f viewSize = m_CurrentState->GetView().getSize();
			const sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;

			m_Background.setScale(scaleFactors);
		}
		else if (type == "SIZE")
		{
			keystream >> m_MaxSize.x >> m_MaxSize.y;
		}
		else if (type == "GRAVITY")
		{
			keystream >> m_Gravity;
		}
		else if (type == "DEFAULT_FRICTION")
		{
			keystream >> m_DefaultTile.Friction.x >> m_DefaultTile.Friction.y;
		}
		else if (type == "NEXTMAP")
		{
			keystream >> m_NextMap;
		}
	}
}

void Map::LoadNext()
{
	m_LoadNextMap = true;
}

void Map::Update(float deltaTime)
{
	if (m_LoadNextMap)
	{
		PurgeEverything();
		m_LoadNextMap = false;

		if (!m_NextMap.empty())
		{
			LoadMap("cfg/Maps/" + m_NextMap);
		}
		else
		{
			m_CurrentState->GetStateManager()->SwitchTo(StateType::GameOver);
		}

		m_NextMap.clear();
	}

	const sf::FloatRect viewSpace = m_Context->GetWindow()->GetViewSpace();
	m_Background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	sf::RenderWindow* window = m_Context->GetWindow()->GetRenderWindow();
	window->draw(m_Background);

	const sf::FloatRect viewSpace = m_Context->GetWindow()->GetViewSpace();

	/*
	*	Examples: 0 to 30 TileID or 340 to 370 TileID, depending on where the view is.
	*	
	*	ViewSpace could be dependent on player (GameState). It could depend on various States.
	*/
	const int beginX = static_cast<int>(std::floor(viewSpace.left / TileSheet::TileSize));
	const int beginY = static_cast<int>(std::floor(viewSpace.top / TileSheet::TileSize));
	const int endX   = static_cast<int>(std::ceil((viewSpace.left + viewSpace.width) / TileSheet::TileSize));
	const int endY   = static_cast<int>(std::ceil((viewSpace.top + viewSpace.height / TileSheet::TileSize)));

	const sf::Vector2i tileBegin(beginX, beginY);
	const sf::Vector2i tileEnd(endX, endY);

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0)
			{
				continue;
			}

			Tile* tile = GetTile(x, y);
			if (!tile)
			{
				continue;
			}

			sf::Sprite& sprite = tile->Properties->Sprite;

			const float posX = static_cast<float>(x * TileSheet::TileSize);
			const float posY = static_cast<float>(y * TileSheet::TileSize);
			sprite.setPosition(posX, posY);

			window->draw(sprite);

			++count;
		}
	}
}

void Map::LoadTiles(const std::string& path)
{
	std::ifstream file;
	file.open(std::filesystem::current_path() / path);
	if (!file.is_open())
	{
		std::cerr << "Failed to load a tile set file: " << path << std::endl;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream keystream(line);
		int tileID;
		keystream >> tileID;
		if (tileID < 0)
		{
			continue;
		}

		TileInfo* tileInfo = new TileInfo(m_Context, "TileSheet", tileID);
		keystream >> tileInfo->Name >> tileInfo->Friction.x >> tileInfo->Friction.y >> tileInfo->bDeadly;

		if (!m_TileSet.emplace(tileID, tileInfo).second)
		{
			std::cerr << "Duplicate detected! Tile type: " << tileInfo->Name << std::endl;
			delete tileInfo;
		}
	}
}

void Map::PurgeEverything()
{
	m_TileCount = 0;
	for (auto& iter : m_Tiles)
	{
		delete iter.second; // deallocating memory for Tile
	}

	m_Tiles.clear();
	//m_Context->GetEntityManager()->PurgeEverything();

	if (m_BackgroundTextureName.empty())
	{
		return;
	}

	m_Context->GetTextureManager()->ReleaseResource(m_BackgroundTextureName);
	m_BackgroundTextureName.clear();
}

void Map::PurgeTileSet()
{
	for (auto& iter : m_TileSet)
	{
		delete iter.second; // deallocating memory for TileInfo
	}

	m_TileSet.clear();
	m_TileSetCount = 0;
}
