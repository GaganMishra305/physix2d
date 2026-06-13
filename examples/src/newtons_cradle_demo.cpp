#include <memory>
#include <vector>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/DistanceJoint.h"
#include "physix2d/Collision.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Newton's cradle: N balls hung from anchors by DistanceJoints, touching at
// rest. The leftmost ball is pulled aside and released; momentum transfers
// through the stationary middle balls and ejects only the far ball.
// A solid stress test of joints + near-elastic restitution together.

const int   WIDTH = 1000, HEIGHT = 700;
const float GRAVITY = 1200.0f;
const int   N_BALLS = 5;
const float R = 25.0f;
const float L = 260.0f;     // string length
const float ANCHOR_Y = 120.0f;
const float RESTITUTION = 0.92f; // near-elastic for clean momentum transfer

std::vector<Body*> anchors, balls;

void setup(World& world) {
    float cx = WIDTH * 0.5f - (N_BALLS - 1) * R;
    std::vector<Body*> targets;
    for (int i = 0; i < N_BALLS; ++i) {
        float x = cx + i * 2.0f * R;
        anchors.push_back(&world.addBody(Body(x, ANCHOR_Y, 4.0f, 0.0f)));  // static
        Body ball(x, ANCHOR_Y + L, R, 1.0f);
        Body& ref = world.addBody(ball);
        balls.push_back(&ref);
        targets.push_back(&ref);
    }
    for (int i = 0; i < N_BALLS; ++i)
        world.addJoint(std::make_unique<DistanceJoint>(anchors[i], balls[i], L));

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY), targets));
    world.setJointIterations(30);

    // Pull the leftmost ball aside (~54 deg) and release from rest.
    float theta = 0.95f;
    balls[0]->setPos(Vec2(anchors[0]->pos.x - L * std::sin(theta),
                          anchors[0]->pos.y + L * std::cos(theta)));
}

void draw(sf::RenderWindow& window, World&) {
    for (size_t i = 0; i < balls.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(anchors[i]->pos.x, anchors[i]->pos.y), sf::Color(150, 150, 150)),
            sf::Vertex(sf::Vector2f(balls[i]->pos.x, balls[i]->pos.y), sf::Color(150, 150, 150))
        };
        window.draw(line, 2, sf::Lines);
    }
    for (Body* b : balls) {
        sf::CircleShape c(b->getRadius());
        c.setPosition(b->pos.x - b->getRadius(), b->pos.y - b->getRadius());
        c.setFillColor(sf::Color(200, 200, 210));
        c.setOutlineThickness(2.0f);
        c.setOutlineColor(sf::Color(90, 90, 110));
        window.draw(c);
    }
}

void step(World&) {
    for (int it = 0; it < 10; ++it)
        for (size_t i = 0; i < balls.size(); ++i)
            for (size_t j = i + 1; j < balls.size(); ++j)
                Collision::resolveCircleVsCircle(*balls[i], *balls[j], RESTITUTION);
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Newton's Cradle");
    setup(engine.getWorld());
    engine.getRenderer().setDrawCallback(draw);
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.run();
}
