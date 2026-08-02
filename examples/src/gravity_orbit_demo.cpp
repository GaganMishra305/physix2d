#include <deque>
#include <vector>
#include <cmath>

#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/GravitationalForce.h"

using namespace physix2d;

// N-Body Gravity: a massive central star with planets on circular and elliptical
// orbits, each attracted by Newtonian gravity. Fading trails trace the paths.

const int WIDTH = 1000, HEIGHT = 800;
const float CX = 500, CY = 400;
const float G = 1.0f, SUN_MASS = 2.0e7f;
const size_t TRAIL = 160;

struct Planet { float r, mass, vfac, angle; sf::Color color; };

std::vector<Body*> bodies;                 // [0] = sun, rest = planets
std::vector<std::deque<Vec2>> trails;
std::vector<sf::Color> colors;

void addPlanet(World& world, const Planet& p) {
    Vec2 pos(CX + p.r * std::cos(p.angle), CY + p.r * std::sin(p.angle));
    float v = std::sqrt(G * SUN_MASS / p.r) * p.vfac;
    Vec2 vel(-std::sin(p.angle) * v, std::cos(p.angle) * v); // tangential
    Body b(pos.x, pos.y, 7.0f, p.mass);
    b.setVel(vel);
    bodies.push_back(&world.addBody(b));
    colors.push_back(p.color);
}

void setup(World& world) {
    // Central star.
    Body sun(CX, CY, 22.0f, SUN_MASS);
    bodies.push_back(&world.addBody(sun));
    colors.push_back(sf::Color(255, 210, 70));

    addPlanet(world, { 120, 40, 1.00f, 0.0f,        sf::Color(90, 180, 255) });
    addPlanet(world, { 200, 60, 1.00f, 2.1f,        sf::Color(120, 230, 150) });
    addPlanet(world, { 290, 55, 1.00f, 4.0f,        sf::Color(255, 140, 90)  });
    addPlanet(world, { 375, 48, 0.82f, 5.4f,        sf::Color(200, 130, 255) }); // elliptical

    trails.resize(bodies.size());

    // Every body attracts every other.
    world.addForce(std::make_unique<GravitationalForce>(bodies, G, 8.0f));
}

void step(World&) {
    for (size_t i = 0; i < bodies.size(); ++i) {
        trails[i].push_back(bodies[i]->pos);
        if (trails[i].size() > TRAIL) trails[i].pop_front();
    }
}

void draw(sf::RenderWindow& window, World&) {
    // Fading trails.
    for (size_t i = 1; i < bodies.size(); ++i) {
        const auto& tr = trails[i];
        if (tr.size() < 2) continue;
        sf::VertexArray strip(sf::LineStrip, tr.size());
        for (size_t k = 0; k < tr.size(); ++k) {
            float a = static_cast<float>(k) / tr.size(); // older -> fainter
            sf::Color c = colors[i];
            c.a = static_cast<sf::Uint8>(200 * a);
            strip[k] = sf::Vertex(sf::Vector2f(tr[k].x, tr[k].y), c);
        }
        window.draw(strip);
    }
    // Bodies.
    for (size_t i = 0; i < bodies.size(); ++i) {
        float r = bodies[i]->getRadius();
        sf::CircleShape c(r);
        c.setPosition(bodies[i]->pos.x - r, bodies[i]->pos.y - r);
        c.setFillColor(colors[i]);
        window.draw(c);
    }
}

int main() {
    Engine engine(WIDTH, HEIGHT, "Physix2D - N-Body Gravity");
    setup(engine.getWorld());
    engine.userLogic = [](World& w, Renderer&, float) { step(w); };
    engine.getRenderer().setDrawCallback(draw);
    engine.run();
}
