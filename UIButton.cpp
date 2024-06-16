#include "UIButton.h"
#include <iostream>

UIButton::UIButton(Style normal, Style hovered, Style selected)
	: m_state{ State::NORMAL },
	m_styles{
		{State::NORMAL, normal}, 
		{State::HOVERED, hovered}, 
		{State::SELECTED, selected}
	}
{
	updateStyle();
}

UIButton::UIButton(Style normal)
{
	m_styles[State::NORMAL] = normal;
	m_styles[State::HOVERED] = normal;
	m_styles[State::SELECTED] = normal;
	updateStyle();
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);
	states.transform *= getTransform();
	auto pos = m_text.getPosition();
	pos.y -= m_text.getLocalBounds().top;
	states.transform.translate(pos);
	target.draw(m_text, states);
}

void UIButton::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			auto e = event.mouseButton;
			sf::Vector2i mousePos {e.x, e.y};

			if (e.button == sf::Mouse::Left)
			{
				mouseClick(mousePos);
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			auto e = event.mouseButton;
			sf::Vector2i mousePos{ e.x, e.y };

			if (e.button == sf::Mouse::Left)
			{
				mouseRelease(mousePos);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			auto e = event.mouseMove;
			sf::Vector2i mousePos{ e.x, e.y };
			mouseMove(mousePos);
			break;
		}
	}
}

void UIButton::shrinkSizeToText()
{
	sf::FloatRect textBounds = m_text.getLocalBounds();
	sf::Vector2f newSize(textBounds.width, textBounds.height);
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
	Style updatedStyle = m_styles[getState()];

	m_text.setFillColor(updatedStyle.fontColor);
	m_text.setOutlineColor(updatedStyle.fontOutlineColor);
	m_text.setOutlineThickness(updatedStyle.fontOutlineThickness);

	setBgColor(updatedStyle.bgColor);
	setBgOutlineColor(updatedStyle.bgOutlineColor);
	setBgOutlineThickness(updatedStyle.bgOutlineThickness);
}

void UIButton::mouseClick(sf::Vector2i mousePos)
{
	if (!contains(mousePos))
	{
		return;
	}

	setState(State::SELECTED);
	if (onClick)
	{
		onClick();
	}
}

void UIButton::mouseRelease(sf::Vector2i mousePos)
{
	if (m_state != State::SELECTED)
	{
		return;
	}

	if (contains(mousePos))
	{
		setState(State::HOVERED);
	}
	else
	{
		setState(State::NORMAL);
	}

	if (onRelease)
	{
		onRelease();
	}
}

void UIButton::mouseMove(sf::Vector2i mousePos)
{
	if (contains(mousePos) && m_state == State::NORMAL)
	{
		mouseEnter();
	}
	else if (!contains(mousePos) && m_state == State::HOVERED)
	{
		mouseLeave();
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
