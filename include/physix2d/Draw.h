#include <SFML/Graphics.hpp>
#include "physix2d/World.h"

namespace physix2d {

class Draw{
private:
    sf::RenderWindow window;

public:
    Draw();
    void run(World& world);
};
}