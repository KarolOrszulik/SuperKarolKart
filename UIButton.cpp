#include "UIButton.h"
#include <iostream>

UIButton::UIButton(Style normal, Style hovered, Style selected)
	: m_state(State::NORMAL)
{
	m_normalStyle = normal;
	m_hoveredStyle = hovered;
	m_selectedStyle = selected;
	updateStyle();
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
			auto e = event.mouseButton;
			sf::Vector2i mousePos {e.x, e.y};

			if (e.button == sf::Mouse::Left && contains(mousePos)
				&& m_state != State::SELECTED)
			{
				mouseClick();
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			auto e = event.mouseButton;
			sf::Vector2i mousePos{ e.x, e.y };

			if (e.button == sf::Mouse::Left 
				&& m_state == State::SELECTED)
			{
				mouseRelease(mousePos);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			auto e = event.mouseMove;
			sf::Vector2i mousePos{ e.x, e.y };

			if (contains(mousePos) 
				&& m_state == State::NORMAL)
			{
				mouseEnter();
			}
			else if(!contains(mousePos) 
				&& m_state == State::HOVERED)
			{
				mouseLeave();
			}
			break;
		}
	}
}

void UIButton::shrinkSizeToText()
{
	sf::FloatRect textBounds = m_text.getLocalBounds();
	sf::Vector2f newSize(textBounds.width + 10, textBounds.height + 10);
	setSize(newSize);
}

void UIButton::setState(State state)
{
	if(m_state == state)
	{
		return;
	}
	
	m_state = state;
	updateStyle();
}

void UIButton::updateStyle()
{
	Style updatedStyle;
	switch (m_state)
	{
	case State::NORMAL:
		updatedStyle = m_normalStyle;
		break;
	case State::HOVERED:
		updatedStyle = m_hoveredStyle;
		break;
	case State::SELECTED:
		updatedStyle = m_selectedStyle;
		break;
	}

	m_text.setFillColor(updatedStyle.fontColor);
	m_text.setOutlineColor(updatedStyle.fontOutlineColor);
	m_text.setOutlineThickness(updatedStyle.fontOutlineThickness);

	setBgColor(updatedStyle.bgColor);
	setBgOutlineColor(updatedStyle.bgOutlineColor);
	setBgOutlineThickness(updatedStyle.bgOutlineThickness);
}

void UIButton::mouseClick()
{
	setState(State::SELECTED);
	if (onClick)
	{
		onClick();
	}
}

void UIButton::mouseRelease(sf::Vector2i mousePos)
{
	if (contains(mousePos)) setState(State::HOVERED);
	else setState(State::NORMAL);
	if (onRelease)
	{
		onRelease();
	}
}

void UIButton::mouseEnter()
{
	setState(State::HOVERED);
	if (onMouseEnter)
	{
		onMouseEnter();
	}
}

void UIButton::mouseLeave()
{
	setState(State::NORMAL);
	if (onMouseLeave)
	{
		onMouseLeave();
	}
}
