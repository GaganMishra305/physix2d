#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include <iostream>


using namespace physix2d;

const int WIDTH = 1000, HEIGHT = 800;
std::deque<Body> particles;

// setup the world
void setup(World &world, Renderer &renderer) {
    particles.push_back(Body(220.0, 20.0, 10.0, 1.0));
    particles.push_back(Body(280, 20, 10, 10000));
    particles.push_back(Body(280, 60, 10, 10000));
    particles.push_back(Body(220, 60, 10, 10000));

    for(Body &particle : particles) {
        world.addBody(particle);
    }
}


// update logic
void update(World& world, Renderer& renderer, float dt) {
    auto& bodies = world.getBodies();
    Vec2 force(0.0, 0.5);
    for(Body particle : bodies) {
        std::cout << "DEBUG" << particle.pos.x << std::endl;

        Vec2 acc(force.x / particle.mass, force.y / particle.mass);
        Vec2 ppos = particle.pos;

        particle.pos = 2*particle.pos - particle.prev_pos + acc * dt * dt;
        particle.prev_pos = ppos;
    }
}

// draw logic
void draw(sf::RenderWindow& window, World& world) {
    auto& bodies = world.getBodies();
    for (const auto& body : bodies) {
        sf::CircleShape circle(body.getRadius());
        Vec2 pos = body.pos;
        circle.setPosition(pos.x - body.getRadius(), pos.y - body.getRadius());
        circle.setFillColor(sf::Color::White);
        circle.setOutlineThickness(1.0f);
        circle.setOutlineColor(sf::Color::White);
        window.draw(circle);
    }
}

int32_t main() {
    Engine engine(WIDTH, HEIGHT);
    World &world = engine.getWorld();
    Renderer &renderer = engine.getRenderer();

    // setup world
    setup(world, renderer);

    // update world
    engine.userLogic = [](World &world, Renderer &renderer, float dt) {
        update(world, renderer, dt);
    };

    // render world
    renderer.setDrawCallback(draw);

    engine.run();
}