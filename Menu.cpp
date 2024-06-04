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
	for (auto const& element : elements)
	{
		if(event.type == sf::Event::MouseMoved)
		{
			event.mouseMove.x -= getPosition().x;
			event.mouseMove.y -= getPosition().y;
		}
		else if(event.type == sf::Event::MouseButtonPressed ||
				event.type == sf::Event::MouseButtonReleased)
		{
			event.mouseButton.x -= getPosition().x;
			event.mouseButton.y -= getPosition().y;
		}

		element->handleEvents(event);
	}
}
