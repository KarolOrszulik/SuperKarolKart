#pragma once

#include "UIElement.h"
#include <functional>

class UIButton
	: public UIElement
{
public:
	enum class State
	{
		NORMAL,
		HOVERED,
		SELECTED
	};

	struct Style
	{
		sf::Color bgColor = {0, 0, 0, 0};
		sf::Color bgOutlineColor = {0, 0, 0, 0};
		float bgOutlineThickness = 0.f;
		sf::Color fontColor = { 255, 255, 255, 255 };
		sf::Color fontOutlineColor = { 0, 0, 0, 255 };
		float fontOutlineThickness = 0.f;
	};

	UIButton() = default;
	UIButton(Style normal, Style hovered, Style selected);

	inline void setText(const std::string& text) 
		{ m_text.setString(text); };

	inline void setFont(const sf::Font& font) 
		{ m_text.setFont(font); };

	inline void setCharacterSize(unsigned int size)
		{ m_text.setCharacterSize(size); };

	State getState() const
		{ return m_state; };

	void setState(State state);

	void handleEvents(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void shrinkSizeToText();

	// may add setters and getters
	std::function<void()> onClick;
	std::function<void()> onRelease;
	std::function<void()> onMouseEnter;
	std::function<void()> onMouseLeave;

	Style m_normalStyle;
	Style m_hoveredStyle;
	Style m_selectedStyle;

protected:
	void updateStyle();
	virtual void mouseClick(sf::Vector2i mousePos);
	virtual void mouseRelease(sf::Vector2i mousePos);
	void mouseMove(sf::Vector2i mousePos);
	void mouseEnter();
	void mouseLeave();

private:
	sf::Text m_text;
	State m_state;

	// use Style please
	using UIElement::setBgColor;
	using UIElement::setBgOutlineColor;
	using UIElement::setBgOutlineThickness;
};

