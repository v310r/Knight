#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf
{
	class RenderWindow;
}

class RectangleDebugger
{
public:
	static void Draw(sf::RenderWindow* window, const sf::FloatRect& rectangle);

	static void SetFillColor(const sf::Color& color);

private:
	static sf::RectangleShape rectangleShape;
};
