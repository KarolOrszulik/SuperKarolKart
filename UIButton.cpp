#include "UIButton.h"
#include <iostream>

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);

	target.draw(m_text, states);
}

void UIButton::handleEvents(sf::Event& event)
{
	using namespace sf;
	if (event.type == Event::MouseButtonPressed)
	{
		auto mbEvent = event.mouseButton;
		if (mbEvent.button == Mouse::Left)
		{
			// TODO zmieniæ na getWidth itd.
			FloatRect bounds = getBackground().getLocalBounds();
			bounds.top += getPosition().y;
			bounds.left += getPosition().x;
			if (bounds.contains({(float)mbEvent.x, (float)mbEvent.y}))
			{
				if (onClick) {
					onClick();
				}
			}
		}
	}
}
