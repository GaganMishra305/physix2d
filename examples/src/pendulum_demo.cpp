#include <memory>
#include <vector>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/DistanceJoint.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Pendulum / rope: a chain of bodies linked by DistanceJoints, hanging from a
// static anchor and released off to the side so it swings. This is the payoff
// for the constraint framework -- and the correct way the cloth should've worked.

const int   WIDTH = 1000, HEIGHT = 800;
const float GRAVITY = 900.0f;
const int   LINKS = 16;
const float LINK_LEN = 32.0f;

std::vector<Body*> chain; // for drawing the rope

void setup(World& world) {
    float startX = WIDTH * 0.5f;
    float startY = 80.0f;

    // Static anchor.
    Body anchor(startX, startY, 6.0f, 0.0f);
    chain.push_back(&world.addBody(anchor));

    std::vector<Body*> targets;
    // Lay the chain out horizontally so it drops and swings.
    for (int i = 1; i <= LINKS; ++i) {
        Body link(startX + i * LINK_LEN, startY, 6.0f, 1.0f);
        link.setDamping(1.2f); // bleed off energy so the rope settles into a natural drape
        Body& ref = world.addBody(link);
        chain.push_back(&ref);
        targets.push_back(&ref);
    }

    // Connect consecutive links with distance joints.
    for (size_t i = 1; i < chain.size(); ++i) {
        world.addJoint(std::make_unique<DistanceJoint>(chain[i - 1], chain[i], LINK_LEN));
    }

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
    world.setJointIterations(20); // stiffer rope
}

void draw(sf::RenderWindow& window, World&) {
    // Rope segments.
    for (size_t i = 1; i < chain.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(chain[i - 1]->pos.x, chain[i - 1]->pos.y), sf::Color(180, 180, 180)),
            sf::Vertex(sf::Vector2f(chain[i]->pos.x, chain[i]->pos.y), sf::Color(180, 180, 180))
        };
        window.draw(line, 2, sf::Lines);
    }
    // Nodes.
    for (size_t i = 0; i < chain.size(); ++i) {
        Body* b = chain[i];
        sf::CircleShape c(b->getRadius());
        c.setPosition(b->pos.x - b->getRadius(), b->pos.y - b->getRadius());
        c.setFillColor(i == 0 ? sf::Color::Red : sf::Color(120, 200, 255));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Pendulum / Rope");
    setup(engine.getWorld());
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
