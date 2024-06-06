#include "UIButton.h"
#include <iostream>

UIButton::UIButton()
	: m_isActive(false), 
	m_isHovered(false)
{
	setBackgroundColor({0,0,0,0});
	setFontColor({255,255,255,255});
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);
	states.transform *= getTransform();
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
		case sf::Event::MouseMoved:
		{
			auto mmEvent = event.mouseMove;
			sf::FloatRect bounds(getPosition(), getSize());
			sf::Vector2f eventBounds((float)mmEvent.x, (float)mmEvent.y);

			if (!m_isHovered && bounds.contains(eventBounds))
			{
				m_isHovered = true;
				onMouseEnter(*this);
			}
			else if (m_isHovered && !bounds.contains(eventBounds))
			{
				m_isHovered = false;
				onMouseLeave(*this);
			}
			break;
		}
	}
}

void UIButton::setText(const std::string& text)
{
	m_text.setString(text);
}

void UIButton::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void UIButton::setFontColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void UIButton::setCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

bool UIButton::isActive() const
{
	return m_isActive;
}

bool UIButton::isHovered() const
{
	return m_isHovered;
}

void UIButton::shrinkSizeToText()
{
	sf::FloatRect textBounds = m_text.getLocalBounds();
	sf::Vector2f newSize(textBounds.width + 10, textBounds.height + 10);
	setSize(newSize);
}

void UIButton::setActive(bool active)
{
	m_isActive = active;
}
