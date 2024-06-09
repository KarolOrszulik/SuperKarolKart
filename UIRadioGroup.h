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
	void addElement(std::unique_ptr<UIToggleButton> element);

private:
	int m_activeElIdx = -1;
	std::vector<std::unique_ptr<UIToggleButton>> m_elements;
};

