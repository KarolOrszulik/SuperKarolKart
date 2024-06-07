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

void Menu::handleEvents(sf::Event& event)
{
	if (event.type == sf::Event::TextEntered)
	{
		sf::Event::TextEvent e = event.text;
		if (e.unicode < 128)
		{ 
			// tutaj zrobic, ze wysyla sie event do elementu z focusem
		}
	}
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		{
			event.mouseButton.x -= getPosition().x;
			event.mouseButton.y -= getPosition().y;

			for (auto const& element : elements)
			{
				element->handleEvents(event);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			event.mouseMove.x -= getPosition().x;
			event.mouseMove.y -= getPosition().y;

			for (auto const& element : elements)
			{
				element->handleEvents(event);
			}
			break;
		}
	}
}
