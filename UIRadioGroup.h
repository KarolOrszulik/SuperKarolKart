#pragma once

#include <vector>
#include <memory>

#include "UIElement.h"
#include "UIToggleButton.h"

class UIRadioGroup
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(sf::Event& event) override;
	void addElement(std::shared_ptr<UIToggleButton> element);
	void addElements(std::vector<std::shared_ptr<UIToggleButton>> elements);
	//void setActiveElement(int idx);

private:
	int m_activeElIdx = -1;
	std::vector<std::shared_ptr<UIToggleButton>> m_elements;
};

