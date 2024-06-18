#include "UIRadioGroup.h"

void UIRadioGroup::handleEvent(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			auto e = event.mouseButton;
			sf::Vector2i mousePos (e.x, e.y);
			if(e.button == sf::Mouse::Left)
			for (int i = 0; i < m_elements.size(); i++)
			{
				if(m_elements[i]->contains(mousePos))
				{
					if(m_activeElIdx != -1)
						m_elements[m_activeElIdx]->setState(UIButton::State::NORMAL);
					m_elements[i]->mouseClick(mousePos);
					m_activeElIdx = i;
					break;
				}
			}
			break;
		}
		case sf::Event::MouseMoved:
		//case sf::Event::MouseButtonReleased: // MO¯LIWE, ¯E ZBÊDNE
		{
			for (auto& e : m_elements)
			{
				e->handleEvent(event);
			}
			break;
		}
	}
}

void UIRadioGroup::addElement(std::shared_ptr<UIToggleButton> element)
{
	m_elements.push_back(element);
}

void UIRadioGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& e : m_elements)
	{
		target.draw(*e, states);
	}
}