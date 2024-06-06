#include "UIElement.h"

sf::Color UIElement::getBackgroundColor() const
{
	return m_background.getFillColor();
}

sf::Vector2f UIElement::getSize() const
{
	return m_background.getSize();
}

float UIElement::getWidth() const
{
	return m_background.getSize().x;
}

float UIElement::getHeight() const
{
	return m_background.getSize().y;
}

void UIElement::setBackgroundColor(const sf::Color& color)
{
	m_background.setFillColor(color);
}

void UIElement::setSize(sf::Vector2f size)
{
	m_background.setSize(size);
}

void UIElement::setWidth(float width)
{
	m_background.setSize(sf::Vector2f(width, m_background.getSize().y));
}

void UIElement::setHeight(float height)
{
	m_background.setSize(sf::Vector2f(m_background.getSize().x, height));
}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
}