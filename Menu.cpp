#include "Menu.h"

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UIElement::draw(target, states);

	for (auto const& element : elements)
	{
		target.draw(*element, getTransform());
	}
}

void Menu::addElement(std::unique_ptr<UIElement> element)
{
	elements.push_back(std::move(element));
}