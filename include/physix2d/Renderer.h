#include <SFML/Graphics.hpp>
#include "physix2d/World.h"

namespace physix2d {

class Renderer{
private:
    int dx = 400, dy = 400; // dimension x and y
    std::string title = "Physix Simulation";
    int fps = 60;
    sf::RenderWindow window;

public:
    Renderer();
    ~Renderer();
    Renderer(int dx_, int dy_, std::string title_ = "Physix Simulation");

    // drawing
    void clear();
    void display();
    void draw(World& world);

    // window management
    bool isOpen() const;
    bool pollEvent(sf::Event& event);
    void close();
};
}