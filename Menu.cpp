#include "Menu.h"
#include <thread>

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
#define paralel
#ifdef paralel
	auto&& lmd = [this, &event](std::shared_ptr<UIElement> el) { el->handleEvent(event); };
	std::vector<std::thread> threads;
#endif
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
#ifdef paralel
	for (auto const& element : m_elements)
	{
		threads.emplace_back(lmd, element);
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
#else
	for (auto const& element : m_elements)
	{
		element->handleEvent(event);
	}
#endif
}
