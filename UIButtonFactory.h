#pragma once

#include "UIButton.h"
#include "UIToggleButton.h"
#include "UITextInput.h"

class UIButtonFactory
{
public:
	UIButtonFactory(
		UIButton::Style normalStyle,
		UIButton::Style hoverStyle,
		UIButton::Style selectedStyle,
		const sf::Font& font,
		int fontSize);

	UIButton createButton(
		const std::string& text,
		const sf::Vector2f& position,
		bool shrinkToText = false,
		const sf::Vector2f& size = { 0, 0 }) const;

	void setCharacterSize(int size) { m_fontSize = size; };

	std::shared_ptr<UIButton> createButtonPtr(
		const std::string& text,
		const sf::Vector2f& position,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UIToggleButton createToggleButton(
		const std::string& text,
		const sf::Vector2f& position,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UIToggleButton> createToggleButtonPtr(
		const std::string& text,
		const sf::Vector2f& position,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UITextInput createTextInput(
		const std::string& text,
		const sf::Vector2f& position,
		const std::string& placeholderText,
		int maxInputLength = -1,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UITextInput> createTextInputPtr(
		const std::string& text,
		const sf::Vector2f& position,
		const std::string& placeholderText,
		int maxInputLength = -1,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

private:
	UIButton::Style m_normStyle;
	UIButton::Style m_hovStyle;
	UIButton::Style m_selStyle;
	const sf::Font& m_font;
	int m_fontSize;
};

