#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/SoftBody.h"

using namespace physix2d;

// Flag in wind: a cloth sheet pinned along its left edge (the flagpole), with a
// gusting wind field pushing it into ripples.

const int WIDTH = 900, HEIGHT = 700;
const float DT = 1.0f / 60.0f;
const int COLS = 20, ROWS = 14;

SoftBody flag;
float t = 0.0f;

void setup() {
    flag = SoftBody::cloth(Vec2(150, 120), 400, 260, COLS, ROWS, 1.0f);
    flag.gravity = Vec2(0.0f, 220.0f);
    flag.damping = 0.02f;
    flag.iterations = 15;
    // Pin the left edge to the "pole".
    for (int y = 0; y <= ROWS; ++y)
        flag.pin(y * (COLS + 1) + 0);
}

void step() {
    t += DT;
    // Gusting wind: base breeze + oscillation + flutter.
    float gust = 380.0f + 260.0f * std::sin(t * 1.7f);
    flag.wind = Vec2(gust, 80.0f * std::sin(t * 5.0f));
    flag.step(DT);
}

void draw(sf::RenderWindow& window, World&) {
    auto idx = [](int x, int y) { return y * (COLS + 1) + x; };
    // Fill each cell as two triangles for a solid flag look.
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x) {
            Vec2 a = flag.points[idx(x, y)].pos;
            Vec2 b = flag.points[idx(x + 1, y)].pos;
            Vec2 c = flag.points[idx(x + 1, y + 1)].pos;
            Vec2 d = flag.points[idx(x, y + 1)].pos;
            sf::Color col = ((x + y) % 2 == 0) ? sf::Color(200, 40, 50) : sf::Color(230, 230, 235);
            sf::Vertex quad[] = {
                sf::Vertex(sf::Vector2f(a.x, a.y), col),
                sf::Vertex(sf::Vector2f(b.x, b.y), col),
                sf::Vertex(sf::Vector2f(c.x, c.y), col),
                sf::Vertex(sf::Vector2f(d.x, d.y), col)
            };
            window.draw(quad, 4, sf::Quads);
        }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Flag in Wind");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { step(); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
