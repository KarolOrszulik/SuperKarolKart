#pragma once
#include "UIButton.h"

class UIToggleButton
	: public UIButton
{
public:
	using UIButton::UIButton;

	bool isToggled() const
		{ return getState() == State::SELECTED; };

	std::function<void()> getOnSelected() const
		{ return m_onSelected; };

	std::function<void()> getOnDeselected() const
		{ return m_onDeselected; };

	void setOnSelected(std::function<void()> onSelected)
		{ m_onSelected = onSelected; };

	void setOnDeselected(std::function<void()> onDeselected)
		{ m_onDeselected = onDeselected; };

	void mouseClick(sf::Vector2i point) override;
	void mouseRelease(sf::Vector2i point) override;

private:
	bool m_IsPressed = false;

	std::function<void()> m_onSelected;
	std::function<void()> m_onDeselected;

	using UIButton::onClick;
	using UIButton::onRelease;
};

