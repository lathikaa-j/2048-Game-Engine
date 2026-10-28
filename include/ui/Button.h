#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

// Non-owning use of the renderer's font; the button owns only geometry and label.
class Button {
    sf::FloatRect bounds;
    std::string label;
public:
    Button(sf::FloatRect area, std::string text) : bounds(area), label(std::move(text)) {}
    bool contains(sf::Vector2f point) const { return bounds.contains(point); }
    void draw(sf::RenderWindow& window, const sf::Font& font) const {
        const auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::RectangleShape shape({bounds.width, bounds.height});
        shape.setPosition(bounds.left, bounds.top);
        shape.setFillColor(window.hasFocus() && contains(mouse)
            ? sf::Color(110, 96, 81) : sf::Color(139, 122, 102));
        window.draw(shape);
        sf::Text text(label, font, 20);
        const auto local = text.getLocalBounds();
        text.setOrigin(local.left + local.width / 2, local.top + local.height / 2);
        text.setPosition(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        text.setFillColor(sf::Color(255, 250, 240));
        window.draw(text);
    }
};
