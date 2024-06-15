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

	inline sf::Color getBgColor() const
		{ return m_background.getFillColor(); };

	inline sf::Vector2f getSize() const 
		{ return m_background.getSize(); };

	inline float getWidth() const
		{ return m_background.getSize().x; };

	inline float getHeight() const
		{ return m_background.getSize().y; };
	

	inline void setBgColor(const sf::Color& color)
		{ m_background.setFillColor(color); };

	inline void setBgOutlineColor(const sf::Color& color)
		{ m_background.setOutlineColor(color); };

	inline void setBgOutlineThickness(float thickness)
		{ m_background.setOutlineThickness(thickness); };

	inline void setSize(sf::Vector2f size)
		{ m_background.setSize(size); };

	inline void setWidth(float width)
		{ m_background.setSize(sf::Vector2f{ width, getHeight() }); };

	inline void setHeight(float height)
		{ m_background.setSize(sf::Vector2f{ getWidth(), height }); };

	bool contains(sf::Vector2f point) const;
	bool contains(sf::Vector2i point) const;

	void centerHorizontally(float parentWidth);
	void centerVertically(float parentHeight);
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void handleEvent(sf::Event& event) = 0;

protected:
	sf::Vector2i translateRelativeToElement(sf::Vector2i globalPoint) const;

private:
	sf::RectangleShape m_background;
};