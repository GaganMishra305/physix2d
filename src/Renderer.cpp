#include "physix2d/Renderer.h"
#include <string>
#include <stdio.h>
#include <iostream>

namespace physix2d {

Renderer::Renderer() : window(sf::VideoMode(dx, dy), title) {
    window.setFramerateLimit(fps);
}

Renderer::Renderer(int dx_, int dy_, std::string title_) : dx(dx_), dy(dy_), title(title_), window(sf::VideoMode(dx, dy), title){
    window.setFramerateLimit(fps);
}


Renderer::~Renderer(){
    if(!window.isOpen()){
        window.close();
    }
}

int Renderer::getFrameCount() const{
    return frameCount;
}

void Renderer::clear() {
    window.clear(sf::Color::Black);
}

void Renderer::display() {
    window.display();
}

void Renderer::draw(World& world) {
    if (customDraw) {
        customDraw(window, world);
    } else {
        // Default rendering
        auto& bodies = world.getBodies();
        for (const auto& body : bodies) {
            sf::CircleShape circle(body.getRadius());
            Vec2 pos = body.getPosition();
            circle.setPosition(pos.x - body.getRadius(), pos.y - body.getRadius());
            circle.setFillColor(sf::Color::White);
            circle.setOutlineThickness(2.0f);
            circle.setOutlineColor(sf::Color::Red);
            window.draw(circle);
        }
    }
    frameCount++;
}

bool Renderer::isOpen() const {
    return window.isOpen();
}

bool Renderer::pollEvent(sf::Event& event) {
    return window.pollEvent(event);
}

void Renderer::close() {
    window.close();
}

}