#pragma once

#include "UIElement.h"
#include <functional>

class UIButton
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void handleEvents(sf::Event& event);

	// function that will be called when the button is clicked
	// gets the button as parameter
	std::function<void(UIButton&)> onClick;
	std::function<void(UIButton&)> onHover;

	void setText(const std::string& text);

	bool isActive() const;
	void setActive(bool active);
	
private:
	sf::Text m_text;
	sf::String m_TextContent;
	bool m_isActive = false;
};

