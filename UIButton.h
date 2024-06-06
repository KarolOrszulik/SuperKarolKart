#pragma once

#include "UIElement.h"
#include <functional>

class UIButton
	: public UIElement
{
public:
	UIButton();

	void setText(const std::string& text);
	void setFont(const sf::Font& font);
	void setFontColor(const sf::Color& color);
	void setActive(bool active);
	void setCharacterSize(unsigned int size);
	
	bool isActive() const;
	bool isHovered() const;

	void shrinkSizeToText();
	void handleEvents(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// may add setters and getters
	std::function<void(UIButton&)> onClick;
	std::function<void(UIButton&)> onMouseEnter;
	std::function<void(UIButton&)> onMouseLeave;
private:
	sf::Text m_text;
	bool m_isActive;
	bool m_isHovered;
};

