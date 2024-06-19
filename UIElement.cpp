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

void UIElement::setSize(sf::Vector2f size)
{
	m_background.setSize(size);
	setOrigin(m_origin);
}

void UIElement::setOrigin(Origin origin)
{
	m_origin = origin;
	switch (origin)
	{
		case Origin::TOP_LEFT:
			Transformable::setOrigin(0, 0);
			break;
		case Origin::TOP_CENTER:
			Transformable::setOrigin(getWidth() / 2.0f, 0);
			break;
		case Origin::TOP_RIGHT:
			Transformable::setOrigin(getWidth(), 0);
			break;
		case Origin::BOT_LEFT:
			Transformable::setOrigin(0, getHeight());
			break;
		case Origin::BOT_CENTER:
			Transformable::setOrigin(getWidth() / 2.0f, getHeight());
			break;
		case Origin::BOT_RIGHT:
			Transformable::setOrigin(getWidth(), getHeight());
			break;
		case Origin::MID_LEFT:
			Transformable::setOrigin(0, getHeight() / 2.0f);
			break;
		case Origin::MID_CENTER:
			Transformable::setOrigin(getWidth() / 2.0f, getHeight() / 2.0f);
			break;
		case Origin::MID_RIGHT:
			Transformable::setOrigin(getWidth(), getHeight() / 2.0f);
			break;
	}
}

bool UIElement::contains(sf::Vector2f point) const
{
	sf::Vector2f origin = Transformable::getOrigin();
	point += origin;
	sf::FloatRect bounds(getPosition(), getSize());
	return bounds.contains(point);
}

bool UIElement::contains(sf::Vector2i point) const
{
	sf::Vector2i origin (Transformable::getOrigin());
	point += origin;
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


