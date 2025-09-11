#include "physix2d/Vec2.h"

namespace physix2d {

Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

float Vec2::getX() const {
    return x;
}

float Vec2::getY() const {
    return y;
}

int Vec2::dot(Vec2 v) {
    return static_cast<int>(x * v.x + y * v.y);
}

Vec2 Vec2::cross(Vec2 v) {
    float cross_product = x * v.y - y * v.x;
    return Vec2(cross_product, cross_product);
}

Vec2 Vec2::add(Vec2 v) {
    return Vec2(x + v.x, y + v.y);
}

}