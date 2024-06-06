#include "UIButton.h"
#include <iostream>

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);

	target.draw(m_text, states);
}

void UIButton::handleEvents(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			auto mbEvent = event.mouseButton;
			if (onClick && mbEvent.button == sf::Mouse::Left)
			{
				sf::FloatRect bounds(getPosition(), getSize());
				sf::Vector2f eventBounds((float)mbEvent.x, (float)mbEvent.y);

				if (bounds.contains(eventBounds))
				{
					m_isActive = true;
					onClick(*this);
				}
				else
				{
					m_isActive = false;
				}
			}
			break;
		}
	}
}

void UIButton::setText(const std::string& text)
{
	
}

bool UIButton::isActive() const
{
	return m_isActive;
}

void UIButton::setActive(bool active)
{
	m_isActive = active;
}
