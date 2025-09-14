#include <iostream>
#include <random>
#include <chrono>
#include "physix2d/Engine.h"
#include "physix2d/Vec2.h"
#include "physix2d/Body.h"

using namespace physix2d;

/* 
CORE DEMO LOGIC IS HERE -> you can modify the variables and play around with the code.
*/
const int WIDTH = 1000, HEIGHT = 800, N_BALLS = 75;
const float DOWNFORCE = 0.5f;
void collisionLogic(World &world) {
    for (Body &b : world.getBodies()) {
        float x = b.getPosition().getX();
        float y = b.getPosition().getY();
        float r = b.getRadius();
        Vec2 v = b.getVel();

        if (y + r > HEIGHT || y - r < 0) b.setVel(Vec2(v.getX(), -v.getY()));
        if (x + r > WIDTH || x - r < 0) b.setVel(Vec2(-v.getX(), v.getY()));
    }
}

void setupWorld(World &world) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> xDist(50.0f, WIDTH * 1.0f);
    std::uniform_real_distribution<float> rDist(15.0f, 35.0f);

    for (int i = 0; i < N_BALLS; ++i) {
        float r = rDist(gen);
        Body c(WIDTH * 0.5f, HEIGHT * 0.5f, r, r / 50.0f);
        c.setVel(Vec2::random2d() * 5.0f);
        world.addBody(c);
        world.addForce(Vec2(0.0f, DOWNFORCE));
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT);

    // CUSTOM USER WORLD AND PHYSICS LOGIC
    setupWorld(engine.getWorld());
    engine.userLogic = [](World &w, Renderer &) { collisionLogic(w); };
    engine.run();
}
