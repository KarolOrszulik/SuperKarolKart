#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class UIElement :
	public sf::Drawable,
	public sf::Transformable
{
public:
	UIElement() = default;
	virtual ~UIElement() = default;

	void setBackground(const sf::RectangleShape& background);
	void setBackgroundColor(const sf::Color& color);
	void setBackgroundSize(sf::Vector2f size);

	sf::RectangleShape getBackground() const;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape m_background;
};