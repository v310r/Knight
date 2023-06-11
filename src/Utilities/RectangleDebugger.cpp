#include "RectangleDebugger.h"

#include <SFML/Graphics/RenderWindow.hpp>


sf::RectangleShape RectangleDebugger::rectangleShape;


void RectangleDebugger::Draw(sf::RenderWindow* window, const sf::FloatRect& rectangle)
{
	rectangleShape.setPosition(rectangle.left, rectangle.top);
	rectangleShape.setSize({ rectangle.width, rectangle.height });

	window->draw(rectangleShape);
}

void RectangleDebugger::SetFillColor(const sf::Color& color)
{
	rectangleShape.setFillColor(color);
}
