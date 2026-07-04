#include "physix2d/Engine.h"
#include "physix2d/SoftBody.h"

using namespace physix2d;

// Rope swing: a rope pinned at the top, released horizontally so it swings down
// and oscillates. Pure position-based dynamics -- stable at any swing speed.

const int WIDTH = 900, HEIGHT = 700;
const float DT = 1.0f / 60.0f;

SoftBody rope;

void setup() {
    // Horizontal rope so it swings down like a whip.
    rope = SoftBody::rope(Vec2(WIDTH * 0.5f, 120.0f), Vec2(WIDTH * 0.5f + 320.0f, 120.0f), 24, 1.0f);
    rope.gravity = Vec2(0.0f, 1000.0f);
    rope.damping = 0.005f;
    rope.iterations = 25;
    rope.pin(0);
    // A heavier "weight" at the free end (smaller invMass) for a satisfying swing.
    rope.points.back().invMass = 0.25f;
}

void draw(sf::RenderWindow& window, World&) {
    for (const auto& c : rope.constraints) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(rope.points[c.a].pos.x, rope.points[c.a].pos.y), sf::Color(200,200,200)),
            sf::Vertex(sf::Vector2f(rope.points[c.b].pos.x, rope.points[c.b].pos.y), sf::Color(200,200,200))
        };
        window.draw(line, 2, sf::Lines);
    }
    for (size_t i = 0; i < rope.points.size(); ++i) {
        float r = (i == 0) ? 7.0f : (i + 1 == rope.points.size() ? 12.0f : 4.0f);
        sf::CircleShape c(r);
        c.setPosition(rope.points[i].pos.x - r, rope.points[i].pos.y - r);
        c.setFillColor(i == 0 ? sf::Color::Red
                     : (i + 1 == rope.points.size() ? sf::Color(255,200,80) : sf::Color(120,200,255)));
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Rope Swing");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { rope.step(DT); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
