#include "GameObject.h"

#include <iostream>

GameObject::GameObject(sf::Vector2f position)
	: m_position(position)
{}

void GameObject::draw(sf::RenderTarget& target)
{
	if (!m_textureLoaded)
	{
		m_textureLoaded = loadTextureWithFallback(m_texture, getTexturePath());
	}

	sf::Sprite sprite(m_texture);
	sprite.setPosition(m_position);
	sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
	target.draw(sprite);
}

bool GameObject::loadTextureWithFallback(sf::Texture& texture, const std::filesystem::path& path, const std::filesystem::path& fallbackPath)
{
	bool success = texture.loadFromFile(path.string());
	bool fallbackSuccess = false;

	if (!success)
	{
		fallbackSuccess = texture.loadFromFile(fallbackPath.string());

		if (!fallbackSuccess)
			throw std::runtime_error("Failed to load texture (even fallback!)");
	}

	return success;
}
