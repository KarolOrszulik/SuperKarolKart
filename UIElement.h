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

	// TODO dodaæ getWidth i getHeight i setWidth i setHeight

	sf::RectangleShape getBackground() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void handleEvents(sf::Event& event) = 0;
private:
	sf::RectangleShape m_background;
};