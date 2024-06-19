#include "GameObject.h"


sf::Uint8 GameObject::checkboard[] = {
	0,  0,   0, 255,
	128,  0, 128, 255,
	128,  0, 128, 255,
	0,  0,  0,  255
};

GameObject::GameObject(sf::Vector2f position)
	: m_position(position)
{
}

void GameObject::draw(sf::RenderTarget& target)
{
	m_sprite.setPosition(m_position);
	target.draw(m_sprite);
}

void GameObject::assignTexture(const sf::Texture& texture)
{
	getTexture();
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
}

const sf::Texture& GameObject::getTexture()
{
	if (m_sprite.getTexture())
	{
		sf::Image img;
		img.create(2, 2, checkboard);
		
		sf::Texture texture;
		texture.loadFromImage(img);
		texture.setRepeated(true);
	}
	return *m_sprite.getTexture();
}