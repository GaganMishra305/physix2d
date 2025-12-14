#include "physix2d/Engine.h"
#include "physix2d/Body.h"
#include "physix2d/GravityForce.h"
#include "physix2d/Collision.h"
#include <iostream>

using namespace physix2d;

const int WIDTH = 1000, HEIGHT = 800;
const float GRAVITY = 100.0;

class Stick {
public:
    Body* a; 
    Body* b;
    float length;
    Stick(Body* a_, Body* b_, float len) : a(a_), b(b_), length(len) {}
};

std::vector<Body*> particles;  // Store pointers to bodies in world
std::deque<Stick> sticks;

float getDistance(Body* a, Body* b){
    Vec2 d = a->pos - b->pos;
    return d.getMag();
}

// setup the world
void setup(World &world, Renderer &renderer) {
    // Create bodies and add to world (returns reference to actual body in world)
    Body pa(220, 20, 5, 10);
    Body pb(280, 20, 5, 10);
    Body pc(220, 60, 5, 10);
    Body pd(280, 60, 5, 10);
    
    Body* pa_ref = &world.addBody(pa);
    Body* pb_ref = &world.addBody(pb);
    Body* pc_ref = &world.addBody(pc);
    Body* pd_ref = &world.addBody(pd);
    
    particles = {pa_ref, pb_ref, pc_ref, pd_ref};

    // sticks
    Stick sab(pa_ref, pb_ref, getDistance(pa_ref, pb_ref));
    Stick sbc(pb_ref, pc_ref, getDistance(pb_ref, pc_ref));
    Stick scd(pc_ref, pd_ref, getDistance(pc_ref, pd_ref));
    Stick sda(pd_ref, pa_ref, getDistance(pd_ref, pa_ref));
    Stick sac(pa_ref, pc_ref, getDistance(pa_ref, pc_ref));
    Stick sbd(pb_ref, pd_ref, getDistance(pb_ref, pd_ref));

    sticks.insert(sticks.end(), {sab, sbc, scd, sda, sac, sbd});

    // forces
    auto gravity = std::make_unique<GravityForce>(
        Vec2(0.0f, GRAVITY),
        particles  // Pass actual body pointers
    );
    world.addForce(std::move(gravity));
}

// update logic
void update(World& world, Renderer& renderer, float dt) {
    std::cout << "DEBUG framecount = " << renderer.getFrameCount() << std::endl;

    // stick restraint
    for(int i =0; i < 5; i++)
    for(Stick &stick : sticks){
        Vec2 diff = stick.a->pos - stick.b->pos;
        float currentDist = diff.getMag();
        float diffFactor = (stick.length - currentDist) / (currentDist) * 0.8f;
        Vec2 offset = diff * diffFactor;

        stick.a->setPos(stick.a->pos + offset);
        stick.b->setPos(stick.b->pos - offset);
    }

    // wall collision
    auto& bodies = world.getBodies();
    for (Body &b : bodies) {
        Collision::resolveWallCollisions(
            b, 5, WIDTH - 5, 5, HEIGHT - 5, 0.8f
        );
    }
}

// draw logic
void draw(sf::RenderWindow& window, World& world) {
    auto& bodies = world.getBodies();

    // draw particles
    for (const auto& body : bodies) {
        sf::CircleShape circle(body.getRadius());
        Vec2 pos = body.pos;
        circle.setPosition(pos.x - body.getRadius(), pos.y - body.getRadius());
        circle.setFillColor(sf::Color::White);
        circle.setOutlineThickness(1.0f);
        circle.setOutlineColor(sf::Color::White);
        window.draw(circle);
    }

    //draw sticks
    for (const auto& stick : sticks) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(stick.a->pos.x, stick.a->pos.y), sf::Color::Cyan),
            sf::Vertex(sf::Vector2f(stick.b->pos.x, stick.b->pos.y), sf::Color::Cyan)
        };
        window.draw(line, 2, sf::Lines);
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