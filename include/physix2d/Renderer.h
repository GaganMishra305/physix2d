#include <SFML/Graphics.hpp>
#include "physix2d/World.h"
#include <functional>

namespace physix2d {

using DrawCallback = std::function<void(sf::RenderWindow&, World&)>;
class Renderer{
private:
    int dx = 400, dy = 400; // dimension x and y
    std::string title = "Physix Simulation";
    int fps = 60;
    int frameCount = 0;
    sf::RenderWindow window;
    DrawCallback customDraw;

public:
    Renderer();
    ~Renderer();
    Renderer(int dx_, int dy_, std::string title_ = "Physix Simulation");
    int getFrameCount() const;

    // drawing
    void clear();
    void display();
    void draw(World& world);
    void setDrawCallback(DrawCallback callback) {
        customDraw = callback;
    }

    // window management
    bool isOpen() const;
    bool pollEvent(sf::Event& event);
    void close();
};
}