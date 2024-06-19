#include "GameObject.h"

#include <iostream>

GameObject::GameObject(sf::Vector2f position)
	: m_position(position)
{}

void GameObject::draw(sf::RenderTarget& target)
{
	if (!m_textureLoaded)
	{
		m_textureLoaded = m_texture.loadFromFile(getTexturePath().string());

		if (!m_textureLoaded)
		{
			std::cerr << "Failed to load texture " << getTexturePath() << ", using notexture.png\n";
			m_textureLoaded = m_texture.loadFromFile("assets/notexture.png");
		}

		if (!m_textureLoaded)
			throw std::runtime_error("Failed to load texture (even notexture!)");
	}

	sf::Sprite sprite(m_texture);
	sprite.setPosition(m_position);
	sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
	target.draw(sprite);
}
