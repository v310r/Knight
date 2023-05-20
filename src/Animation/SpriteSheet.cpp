#include "SpriteSheet.h"
#include "AnimDirectional.h"

SpriteSheet::SpriteSheet(TextureManager* const textureManager)
	: m_TextureManager(textureManager)
{

}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::CropSprite(const sf::IntRect& rect)
{
	m_Sprite.setTextureRect(rect);
}

bool SpriteSheet::LoadSheet(const std::string& configPath)
{
	std::ifstream sheet;
	sheet.open(std::filesystem::current_path() / configPath);
	if (sheet.is_open())
	{
		ReleaseSheet();
		std::string line;
		while (std::getline(sheet, line))
		{
			if (line[0] == '|')
			{
				continue;
			}
			std::stringstream keystream(line);
			std::string type;
			keystream >> type;

			if (type == "Texture")
			{
				if (m_TextureName != "")
				{
					std::cerr << "Duplicate texture entries in: " << configPath << std::endl;
					continue;
				}

				std::string textureName;
				keystream >> textureName;
				if (!m_TextureManager->RequireResource(textureName))
				{
					std::cerr << "Couldn't set up the texture: " << textureName << std::endl;
					continue;
				}

				m_TextureName = textureName;
				m_Sprite.setTexture(*m_TextureManager->GetResource(m_TextureName));
			}
			else if (type == "Size")
			{
				keystream >> m_SpriteSize.x >> m_SpriteSize.y;
				SetSpriteSize(m_SpriteSize);
			}
			else if (type == "Scale")
			{
				keystream >> m_SpriteScale.x >> m_SpriteScale.y;
				m_Sprite.setScale(m_SpriteScale);
			}
			else if (type == "AnimationType")
			{
				keystream >> m_AnimType;
			}
			else if (type == "Animation")
			{
				std::string name;
				keystream >> name;
				if (m_Animations.find(name) != m_Animations.end())
				{
					std::cerr << "Duplicate animation (" << name << ") in: " << configPath << std::endl;
				}

				AnimBase* anim = nullptr;
				if (m_AnimType == "Directional")
				{
					anim = new AnimDirectional();
				}
				else
				{
					std::cerr << "Unknown animation type: " << m_AnimType << std::endl;
					continue;
				}

				keystream >> *anim;
				anim->SetSpriteSheet(this);
				anim->SetName(name);
				anim->Reset();
				m_Animations.emplace(name, anim);
			}
		}

		if (auto iter = m_Animations.find(m_DefaultAnimationName); iter  != m_Animations.end())
		{
			m_CurrentAnimation = iter->second;
		}
		else if (auto iter = m_Animations.begin(); iter != m_Animations.end())
		{
			m_CurrentAnimation = iter->second;
		}
		else
		{
			std::cerr << "No Animation settings present in config file: " << configPath << std::endl;
			return false;
		}

		m_CurrentAnimation->Play();

		return true;
	}

	std::cerr << "Failed to load SpriteSheet: " << configPath << std::endl;
	return false;
}

void SpriteSheet::ReleaseSheet()
{
	m_TextureManager->ReleaseResource(m_TextureName);
	m_CurrentAnimation = nullptr;
	while (m_Animations.begin() != m_Animations.end())
	{
		delete m_Animations.begin()->second;
		m_Animations.erase(m_Animations.begin());
	}
}

void SpriteSheet::Update(float deltaTime)
{
	m_CurrentAnimation->Update(deltaTime);
}

void SpriteSheet::Draw(sf::RenderWindow& window)
{
	window.draw(m_Sprite);
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size)
{
	m_SpriteSize = size;
	m_Sprite.setOrigin(m_SpriteSize.x / 2.0f, m_SpriteSize.y / 2.0f);
}

sf::Vector2i SpriteSheet::GetSpriteSize()
{
	return m_SpriteSize;
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f pos)
{
	m_Sprite.setPosition(pos);
}

void SpriteSheet::SetDirection(Direction dir)
{
	if (dir == m_SpriteDirection)
	{
		return;
	}
	m_SpriteDirection = dir;
	m_CurrentAnimation->CropSprite();
}

Direction SpriteSheet::GetDirection()
{
	return m_SpriteDirection;
}

bool SpriteSheet::SetAnimation(const std::string& name, bool bPlay, bool bLoop)
{
	auto iter = m_Animations.find(name);
	if (iter == m_Animations.end())
	{
		return false;
	}

	if (iter->second == m_CurrentAnimation)
	{
		return false;
	}

	if (m_CurrentAnimation)
	{
		m_CurrentAnimation->Stop();
	}

	m_CurrentAnimation = iter->second;
	m_CurrentAnimation->SetLooping(bLoop);
	if (bPlay)
	{
		m_CurrentAnimation->Play();
	}
	m_CurrentAnimation->CropSprite();
	return true;


}
