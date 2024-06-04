#pragma once

#include "UIElement.h"
#include <functional>

class UIButton
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void handleEvents(sf::Event& event);

	std::function<void()> onClick;
		
private:
	sf::Text m_text;
};

