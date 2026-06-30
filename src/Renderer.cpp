#include "physix2d/Renderer.h"
#include "physix2d/CircleShape.h"
#include "physix2d/PolygonShape.h"
#include <cmath>
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
        // Default rendering: dispatch on shape type (circle or polygon).
        auto& bodies = world.getBodies();
        for (const auto& body : bodies) {
            Shape* sh = body.getShape();
            if (sh && sh->getType() == ShapeType::Polygon) {
                auto* poly = static_cast<PolygonShape*>(sh);
                sf::ConvexShape cs;
                cs.setPointCount(poly->vertices.size());
                float c = std::cos(body.angle), s = std::sin(body.angle);
                Vec2 p = body.getPosition();
                for (size_t i = 0; i < poly->vertices.size(); ++i) {
                    const Vec2& v = poly->vertices[i];
                    float wx = p.x + (v.x * c - v.y * s);
                    float wy = p.y + (v.x * s + v.y * c);
                    cs.setPoint(i, sf::Vector2f(wx, wy));
                }
                cs.setFillColor(sf::Color(120, 180, 255));
                cs.setOutlineThickness(1.5f);
                cs.setOutlineColor(sf::Color::White);
                window.draw(cs);
            } else {
                sf::CircleShape circle(body.getRadius());
                Vec2 pos = body.getPosition();
                circle.setPosition(pos.x - body.getRadius(), pos.y - body.getRadius());
                circle.setFillColor(sf::Color::White);
                circle.setOutlineThickness(2.0f);
                circle.setOutlineColor(sf::Color::Red);
                window.draw(circle);
            }
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