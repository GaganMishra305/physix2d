#include "physix2d/Engine.h"
#include "physix2d/SoftBody.h"

using namespace physix2d;

// Jelly cube: an unpinned soft-body grid with slightly soft constraints so it
// squishes on impact and wobbles back -- dropped with a sideways velocity so it
// tumbles around the box.

const int WIDTH = 800, HEIGHT = 600;
const float DT = 1.0f / 60.0f;
const int N = 6; // grid resolution

SoftBody jelly;

void setup() {
    jelly = SoftBody::cloth(Vec2(120, 80), 140, 140, N, N, 1.0f);
    jelly.gravity = Vec2(0.0f, 900.0f);
    jelly.damping = 0.01f;
    jelly.iterations = 12;
    jelly.hasBounds = true;
    jelly.bounds = AABB{ Vec2(20, 20), Vec2(WIDTH - 20, HEIGHT - 20) };

    // Soften structural springs so it jiggles like jelly (not stiff like cloth).
    for (auto& c : jelly.constraints) c.stiffness *= 0.7f;

    // Give it an initial sideways+down velocity (Verlet: encode via prev).
    Vec2 v0(260.0f, 120.0f);
    for (auto& p : jelly.points) p.prev = p.pos - v0 * DT;
}

void draw(sf::RenderWindow& window, World&) {
    auto idx = [](int x, int y) { return y * (N + 1) + x; };
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x) {
            Vec2 a = jelly.points[idx(x, y)].pos;
            Vec2 b = jelly.points[idx(x + 1, y)].pos;
            Vec2 c = jelly.points[idx(x + 1, y + 1)].pos;
            Vec2 d = jelly.points[idx(x, y + 1)].pos;
            sf::Vertex quad[] = {
                sf::Vertex(sf::Vector2f(a.x, a.y), sf::Color(120, 220, 160)),
                sf::Vertex(sf::Vector2f(b.x, b.y), sf::Color(120, 220, 160)),
                sf::Vertex(sf::Vector2f(c.x, c.y), sf::Color(90, 190, 140)),
                sf::Vertex(sf::Vector2f(d.x, d.y), sf::Color(90, 190, 140))
            };
            window.draw(quad, 4, sf::Quads);
        }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - Jelly Cube");
    setup();
    engine.userLogic = [](World&, Renderer&, float) { jelly.step(DT); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
