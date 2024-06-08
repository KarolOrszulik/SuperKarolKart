#include "UIToggleButton.h"

void UIToggleButton::mouseClick(sf::Vector2i point)
{
	if (!contains(point))
	{
		return;
	}

	if (getState() == State::SELECTED)
	{
		setState(State::HOVERED);
		if (onDeselected)
		{
			onDeselected();
		}
	}
	else
	{
		setState(State::SELECTED);
		if(onSelected)
		{
			onSelected();
		}
	}
	m_IsPressed = true;

}

void UIToggleButton::mouseRelease(sf::Vector2i point)
{
	if (m_IsPressed && onRelease)
	{
		onRelease();
		m_IsPressed = false;
	}
}
