#include "Menu.h"

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);

	for (auto const& element : m_elements)
	{
		element->draw(target, getTransform());
	}
}

void Menu::addElement(std::shared_ptr<UIElement> element)
{
	m_elements.push_back(element);
}

void Menu::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		{
			event.mouseButton.x -= static_cast<int>(getPosition().x);
			event.mouseButton.y -= static_cast<int>(getPosition().y);
			break;
		}
		case sf::Event::MouseMoved:
		{
			event.mouseMove.x -= static_cast<int>(getPosition().x);
			event.mouseMove.y -= static_cast<int>(getPosition().y);
		}
	}
	for (auto const& element : m_elements)
	{
		element->handleEvent(event);
	}
}
