#include "UIImage.h"

UIImage::UIImage(Style normal, const sf::Texture& texture)
	: UIButton(normal)
{
	assignTexture(texture);
}

void UIImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}

void UIImage::assignTexture(const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

void UIImage::expandImageToSize()
{
	auto m_size = getSize();
	m_sprite.setScale(m_size.x / m_sprite.getTexture()->getSize().x, 
		m_size.y / m_sprite.getTexture()->getSize().y);
}

void UIImage::applyPadding(unsigned padding)
{
	m_padding = padding;
	sf::Vector2f paddingVec(padding, padding);
	sf::Vector2f size = getSize();
	size += paddingVec * 2.f;
	setSize(size);
	m_sprite.move(paddingVec);
}

const sf::Texture& UIImage::getTexture()
{
	return *m_sprite.getTexture();
}
