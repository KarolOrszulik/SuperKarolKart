#pragma once
#include "UIButton.h"

class UIToggleButton
	: public UIButton
{
	using UIButton::UIButton;

protected:
	void mouseClick(sf::Vector2i point) override;
	void mouseRelease(sf::Vector2i point) override;
private:
	bool m_IsToggled;
};

