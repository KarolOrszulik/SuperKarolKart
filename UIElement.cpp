#include "UIElement.h"
#include <iostream>

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
}

sf::Vector2i UIElement::translateRelativeToElement(sf::Vector2i globalPoint) const
{
	return globalPoint - sf::Vector2i{(int)getPosition().x, (int)getPosition().y};
}

bool UIElement::contains(sf::Vector2f point) const
{
	sf::FloatRect bounds(getPosition(), getSize());
	return bounds.contains(point);
}

bool UIElement::contains(sf::Vector2i point) const
{
	sf::IntRect bounds(
		{ (int)getPosition().x, (int)getPosition().y },
		{ (int)getWidth(), (int)getHeight()}
	);
	return bounds.contains(point);
}

void UIElement::centerHorizontally(float parentWidth)
{
	setPosition({ (parentWidth - getWidth()) / 2.0f , getPosition().y});
}

void UIElement::centerVertically(float parentHeight)
{
	setPosition({ getPosition().x, (parentHeight - getHeight()) / 2.0f });
}


