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

void UIRadioGroup::addElements(std::vector<std::shared_ptr<UIToggleButton>> elements)
{
	for (auto& e : elements)
	{
		m_elements.push_back(e);
	}
}

void UIRadioGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	UIElement::draw(target, states);

	for (auto const& element : m_elements)
	{
		element->draw(target, getTransform());
	}
}