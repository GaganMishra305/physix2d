#include "physix2d/Vec2.h"
#include <random>

namespace physix2d {

Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

float Vec2::getMag() const {
    return sqrtf(x*x + y*y);
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

Vec2 Vec2::operator+(const Vec2& other) {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) {
    return Vec2(x * scalar, y * scalar);
}

Vec2 operator*(float scalar, const Vec2& vec) {
    return Vec2(vec.x * scalar, vec.y * scalar);
}

Vec2& Vec2::operator=(const Vec2& other) {
    if (this != &other) { // Self-assignment check
        x = other.x;
        y = other.y;
    }
    return *this;
}

Vec2 Vec2::random2d(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> real_distrib(-1.0, 1.0);
    return Vec2(real_distrib(gen), real_distrib(gen));
}

}