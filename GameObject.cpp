#include "GameObject.h"

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
	return *m_sprite.getTexture();
}