#pragma once

#include "UIButton.h"
#include "UIToggleButton.h"
#include "UIRadioGroup.h"
#include "UITextInput.h"
#include "UIImage.h"

class UIElementFactory
{
public:
	UIElementFactory(
		UIButton::Style normalStyle,
		UIButton::Style hoverStyle,
		UIButton::Style selectedStyle,
		const sf::Font& font,
		int fontSize);

	UIElementFactory(
		UIButton::Style normalStyle,
		const sf::Font& font,
		int fontSize);

	UIButton makeBtn(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	void setCharacterSize(int size) { m_fontSize = size; };

	std::shared_ptr<UIButton> makeBtnPtr(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UIImage makeImg(
		const sf::Texture& texture,
		const sf::Vector2f& position = {0, 0},
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		unsigned padding = 0,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UIImage> makeImgPtr(
		const sf::Texture& texture,
		const sf::Vector2f& position = {0, 0},
		unsigned padding = 0,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		const sf::Vector2f& size = { 0, 0 }) const;

	UIToggleButton makeToggle(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UIToggleButton> makeTogglePtr(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	UITextInput makeTxtInp(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
		const std::string& placeholderText = "",
		int maxInputLength = -1,
		UIElement::Origin origin = UIElement::Origin::TOP_LEFT,
		bool shrinkToText = true,
		const sf::Vector2f& size = { 0, 0 }) const;

	std::shared_ptr<UITextInput> makeTxtInpPtr(
		const std::string& text,
		const sf::Vector2f& position = {0, 0},
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

