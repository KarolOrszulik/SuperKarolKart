#include "UIElement.h"

void UIElement::setBackground(const sf::RectangleShape& background)
{
	m_background = background;
}

void UIElement::setBackgroundColor(const sf::Color& color)
{
	m_background.setFillColor(color);
}

void UIElement::setBackgroundSize(sf::Vector2f size)
{
	m_background.setSize(size);
}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
}

sf::RectangleShape UIElement::getBackground() const
{
	return m_background;
}