#include "UITextInput.h"

void UITextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIButton::draw(target, states);
	if (m_text.getString().isEmpty())
	{
		states.transform *= getTransform();

		sf::Text text = m_text;
		text.setString(m_placeHolder);
		auto pos = text.getPosition();
		pos.y -= text.getLocalBounds().top;
		states.transform.translate(pos);
		target.draw(text, states);
	}
}

void UITextInput::handleEvent(sf::Event& event)
{
	UIButton::handleEvent(event);
	if (event.type == sf::Event::TextEntered)
	{
		textEntered(event.text.unicode);
	}
}

void UITextInput::mouseClick(sf::Vector2i mousePos)
{
	if (!contains(mousePos))
	{
		if (getState() == State::SELECTED)
		{
			setState(State::NORMAL);
		}
		return;
	}

	setState(State::SELECTED);
	if (onClick)
	{
		onClick();
	}
}

void UITextInput::mouseRelease(sf::Vector2i mousePos)
{
	if (getState() == State::SELECTED && onRelease)
	{
		onRelease();
	}
}

void UITextInput::textEntered(sf::Uint32 unicode)
{
	if (getState() == State::SELECTED)
	{
		constexpr int backspaceCode = 8;
		std::string text = m_text.getString();
		if (unicode == backspaceCode)
		{
			if (!text.empty()) text.pop_back();
		}
		else
		{
			text += std::toupper(unicode);
		}
		m_text.setString(text);

		if (onTextEntered)
		{
			onTextEntered(getText());
		}
	}
}

void UITextInput::shrinkSizeToText()
{
	if (m_placeHolder.size() > m_text.getString().getSize())
	{
		sf::Text text = m_text;
		text.setString(m_placeHolder);
		sf::FloatRect textBounds = text.getLocalBounds();
		sf::Vector2f newSize(textBounds.width, textBounds.height);
		setSize(newSize);
	}
}

std::string UITextInput::getText() const
{
	return m_text.getString();
}
