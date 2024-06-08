#pragma once
#include "UIButton.h"

class UIToggleButton
	: public UIButton
{
public:
	using UIButton::UIButton;

	bool isToggled() const
		{ return getState() == State::SELECTED; };


	std::function<void()> onSelected;
	std::function<void()> onDeselected;

protected:
	void mouseClick(sf::Vector2i point) override;
	void mouseRelease(sf::Vector2i point) override;

private:
	bool m_IsPressed = false;
	using UIButton::onClick;
	using UIButton::onRelease;
};

