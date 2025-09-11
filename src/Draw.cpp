#include "physix2d/Draw.h"

namespace physix2d {

Draw::Draw() : window(sf::VideoMode(800, 600), "Physix2D Simulation") {
    window.setFramerateLimit(60);
}

void Draw::run(World& world) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update physics
        world.update(1.0f/60.0f);
        // Clear window
        window.clear(sf::Color::Black);
        // Draw all bodies
        auto& bodies = world.getBodies();
        for (const auto& body : bodies) {
            sf::CircleShape circle(body.getRadius());
            Vec2 pos = body.getPosition();
            circle.setPosition(pos.getX() - body.getRadius(), pos.getY() - body.getRadius());
            circle.setFillColor(sf::Color::White);
            circle.setOutlineThickness(2.0f);
            circle.setOutlineColor(sf::Color::Red);
            window.draw(circle);
        }
        
        window.display();
    }
}

}