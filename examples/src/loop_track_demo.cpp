#include <memory>
#include <vector>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/Collision.h"
#include "physix2d/SweepAndPrune.h"
#include "physix2d/GravityForce.h"

using namespace physix2d;

// Loop track: a ball races around the INSIDE of a circular loop, held onto the
// track through the top by centripetal contact (it only stays up because it's
// fast enough -- v^2 >= 5gR). Tuned headless: R=130, entry speed 1200.

const int   WIDTH = 800, HEIGHT = 600;
const float GRAVITY = 900.0f;
const float CX = 400, CY = 300, R = 130, SEG_R = 8;
const int   N_SEG = 100;
const float BALL_R = 11;

Body* ball = nullptr;

void setup(World& world) {
    // Circular track made of static circle segments.
    for (int i = 0; i < N_SEG; ++i) {
        float a = 2.0f * M_PI * i / N_SEG;
        world.addBody(Body(CX + R * std::cos(a), CY + R * std::sin(a), SEG_R, 0.0f));
    }
    // Ball at the bottom of the loop, launched sideways fast enough to loop.
    float orbit = R - SEG_R - BALL_R;
    Body b(CX, CY + orbit, BALL_R, 1.0f);
    b.setFriction(0.02f);
    b.allowSleep = false;
    b.setVel(Vec2(1200.0f, 0.0f));
    ball = &world.addBody(b);

    world.addForce(std::make_unique<GravityForce>(Vec2(0.0f, GRAVITY),
                                                  std::vector<Body*>{ ball }));
}

void step(World& world) {
    auto& bodies = world.getBodies();
    auto pairs = SweepAndPrune::computePairs(bodies);
    for (int it = 0; it < 12; ++it)
        for (auto& p : pairs)
            Collision::resolve(bodies[p.first], bodies[p.second], 0.1f);
}

void draw(sf::RenderWindow& window, World& world) {
    for (auto& b : world.getBodies()) {
        sf::CircleShape c(b.getRadius());
        c.setPosition(b.pos.x - b.getRadius(), b.pos.y - b.getRadius());
        if (&b == ball) c.setFillColor(sf::Color(255, 90, 90));      // the racer
        else            c.setFillColor(sf::Color(90, 90, 110));      // the track
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Loop Track");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
