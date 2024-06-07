#pragma once
#include "UIButton.h"

class UIToggleButton
	: public UIButton
{
	virtual void handleEvents(sf::Event& event);
private:
	bool m_IsToggled;

};

