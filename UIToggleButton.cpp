#include "UIToggleButton.h"

void UIToggleButton::mouseClick(sf::Vector2i point)
{
	if (!contains(point))
	{
		return;
	}

	if (m_IsToggled)
	{
		setState(State::HOVERED);
	}
	else
	{
		setState(State::SELECTED);
	}
	m_IsToggled = !m_IsToggled;

	if (onClick)
	{
		onClick();
	}
}

void UIToggleButton::mouseRelease(sf::Vector2i point)
{
	if (onRelease)
	{
		onRelease();
	}
}
