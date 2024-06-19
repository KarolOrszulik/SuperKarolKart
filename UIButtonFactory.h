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

	UIButton makeBtn(
		const std::string& text,
		const sf::Vector2f& position,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = false,
		const sf::Vector2f& size = { 0, 0 }) const;

	void setCharacterSize(int size) { m_fontSize = size; };

	std::shared_ptr<UIButton> makeBtnPtr(
		const std::string& text,
		const sf::Vector2f& position,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UIToggleButton makeToggle(
		const std::string& text,
		const sf::Vector2f& position,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UIToggleButton> makeTogglePtr(
		const std::string& text,
		const sf::Vector2f& position,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UITextInput makeTxtInp(
		const std::string& text,
		const sf::Vector2f& position,
		const std::string& placeholderText = "",
		int maxInputLength = -1,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UITextInput> makeTxtInpPtr(
		const std::string& text,
		const sf::Vector2f& position,
		const std::string& placeholderText = "",
		int maxInputLength = -1,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

private:
	UIButton::Style m_normStyle;
	UIButton::Style m_hovStyle;
	UIButton::Style m_selStyle;
	const sf::Font& m_font;
	int m_fontSize;
};

