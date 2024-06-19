#include "UIElementFactory.h"

UIElementFactory::UIElementFactory(
	UIButton::Style normStyle, 
	UIButton::Style hovStyle, 
	UIButton::Style selStyle, 
	const sf::Font& font, int fontSize)
	:	m_normStyle(normStyle), m_hovStyle(hovStyle), m_selStyle(selStyle),
		m_font(font), m_fontSize(fontSize)
{
}

UIElementFactory::UIElementFactory(
	UIButton::Style normStyle,
	const sf::Font& font,
	int fontSize)
	: m_normStyle(normStyle), m_hovStyle(normStyle), m_selStyle(normStyle),
	m_font(font), m_fontSize(fontSize)
{}

UIButton UIElementFactory::makeBtn(
	const std::string& text, 
	const sf::Vector2f& position, 
	UIElement::Origin origin,
	bool shrinkToText,
	const sf::Vector2f& size) const
{
	UIButton btn(m_normStyle, m_hovStyle, m_selStyle);
	btn.setText(text);
	btn.setFont(m_font);
	btn.setCharacterSize(m_fontSize);
	btn.setOrigin(origin);
	btn.setPosition(position);
	if (shrinkToText) btn.shrinkSizeToText();
	return btn;
}

#include <iostream>

std::shared_ptr<UIButton> UIElementFactory::makeBtnPtr(
	const std::string& text,
	const sf::Vector2f& position,
	UIElement::Origin origin,
	bool shrinkToText,
	const sf::Vector2f& size) const
{
	auto btn = std::make_shared<UIButton>(m_normStyle, m_hovStyle, m_selStyle);
	btn->setText(text);
	btn->setFont(m_font);
	btn->setCharacterSize(m_fontSize);
	btn->setOrigin(origin);
	btn->setPosition(position);
	if (shrinkToText) btn->shrinkSizeToText();
	return btn;
}


UIToggleButton UIElementFactory::makeToggle(
	const std::string& text, 
	const sf::Vector2f& position, 
	UIElement::Origin origin,
	bool shrinkToText,
	const sf::Vector2f& size) const
{
	UIToggleButton btn(m_normStyle, m_hovStyle, m_selStyle);
	btn.setText(text);
	btn.setFont(m_font);
	btn.setCharacterSize(m_fontSize);
	btn.setOrigin(origin);
	btn.setPosition(position);
	if (shrinkToText) btn.shrinkSizeToText();
	return btn;
}


std::shared_ptr<UIToggleButton> UIElementFactory::makeTogglePtr(
	const std::string& text, 
	const sf::Vector2f& position, 
	UIElement::Origin origin,
	bool shrinkToText, 
	const sf::Vector2f& size) const
{
	auto btn = std::make_shared<UIToggleButton>(m_normStyle, m_hovStyle, m_selStyle);
	btn->setText(text);
	btn->setFont(m_font);
	btn->setCharacterSize(m_fontSize);
	btn->setOrigin(origin);
	btn->setPosition(position);
	if (shrinkToText) btn->shrinkSizeToText();
	return btn;
}

UITextInput UIElementFactory::makeTxtInp(
	const std::string& text, 
	const sf::Vector2f& position, 
	const std::string& placeholderText,
	int maxInputLength,
	UIElement::Origin origin,
	bool shrinkToText, 
	const sf::Vector2f& size) const
{
	UITextInput textInput(m_normStyle, m_hovStyle, m_selStyle);
	textInput.setText(text);
	textInput.setFont(m_font);
	textInput.setCharacterSize(m_fontSize);
	textInput.setPlaceHolder(placeholderText);
	textInput.setOrigin(origin);
	textInput.setPosition(position);
	if (shrinkToText) 
		textInput.shrinkSizeToText();
	
	if (maxInputLength == -1)
		textInput.setMaxLength(static_cast<int>(placeholderText.size()));
	else
		textInput.setMaxLength(maxInputLength);
	return textInput;
}

std::shared_ptr<UITextInput> UIElementFactory::makeTxtInpPtr(
	const std::string& text, 
	const sf::Vector2f& position, 
	const std::string& placeholderText,
	int maxInputLength,
	UIElement::Origin origin,
	bool shrinkToText, 
	const sf::Vector2f& size) const
{
	auto textInput = std::make_shared<UITextInput>(m_normStyle, m_hovStyle, m_selStyle);
	textInput->setText(text);
	textInput->setFont(m_font);
	textInput->setCharacterSize(m_fontSize);
	textInput->setPlaceHolder(placeholderText);
	textInput->setOrigin(origin);
	textInput->setPosition(position);
	if (shrinkToText) 
		textInput->shrinkSizeToText();

	if (maxInputLength == -1)
		textInput->setMaxLength(static_cast<int>(placeholderText.size()));
	else
		textInput->setMaxLength(maxInputLength);
	return textInput;
}