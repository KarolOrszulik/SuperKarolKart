#include "GameObject.h"


GameObject::GameObject(sf::Vector2f position)
	: m_position(position)
{
}

void GameObject::draw(sf::RenderTarget& target)
{
	const sf::Texture& texture = getTexture();
	sf::Sprite sprite(getTexture());
	sprite.setPosition(m_position);
	sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
	target.draw(sprite);
}

void GameObject::assignTexture(const sf::Texture& texture)
{
	m_texture = texture;
}

const sf::Texture& GameObject::getTexture()
{
	if (!m_texture.has_value())
	{
		m_texture.emplace();
		auto& texture = m_texture.value();
		sf::Uint8 checkboard[] = {
			  0,  0,   0, 255,
			128,  0, 128, 255,
			128,  0, 128, 255,
			  0,  0,  0,  255
		};

		sf::Image img;
		img.create(2, 2, checkboard);
		texture.loadFromImage(img);
		texture.setRepeated(true);
	}
	else
	{
		auto& texture = m_texture.value();
		texture = m_texture.value();
		texture.setRepeated(false);
	}
	return m_texture.value();
}