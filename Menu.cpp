#include "Menu.h"

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);

	for (auto const& element : elements)
	{
		element->draw(target, getTransform());
	}
}

void Menu::addElement(std::unique_ptr<UIElement> element)
{
	elements.push_back(std::move(element));
}

void Menu::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::TextEntered:
		{
			sf::Event::TextEvent e = event.text;
			if (e.unicode < 128)
			{
				// tutaj zrobic, ze wysyla sie event do elementu z focusem
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		{
			event.mouseButton.x -= static_cast<int>(getPosition().x);
			event.mouseButton.y -= static_cast<int>(getPosition().y);

			for (auto const& element : elements)
			{
				element->handleEvent(event);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			event.mouseMove.x -= static_cast<int>(getPosition().x);
			event.mouseMove.y -= static_cast<int>(getPosition().y);

			for (auto const& element : elements)
			{
				element->handleEvent(event);
			}
			break;
		}
	}
}
