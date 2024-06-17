#pragma once

#include "UIButton.h"

class UITextInput
	: public UIButton
{
public:
	using UIButton::UIButton;
	~UITextInput() = default;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(sf::Event& event) override;

	void setPlaceHolder(const std::string& placeHolder) { m_placeHolder = placeHolder; }
	void setMaxLength(int maxLength) { m_maxLength = maxLength; }

	std::string getPlaceHolder() const { return m_placeHolder; }
	std::string getText() const;

	std::function<void(const std::string&)> onTextEntered;

	void shrinkSizeToText() override;

protected:
	void mouseClick(sf::Vector2i mousePos) override;
	void mouseRelease(sf::Vector2i mousePos) override;
	void textEntered(sf::Uint32 unicode);

private:
	std::string m_placeHolder;
	int m_maxLength = 0;
};

