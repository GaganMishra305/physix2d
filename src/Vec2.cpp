#include "physix2d/Vec2.h"
#include <cmath>
#include <random>

namespace physix2d {

Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

float Vec2::lengthSq() const {
    return x * x + y * y;
}

float Vec2::getMag() const {
    return std::sqrt(lengthSq());
}

Vec2 Vec2::normalized() const {
    float m = getMag();
    if (m <= 1e-8f) return Vec2(0.0f, 0.0f);
    return Vec2(x / m, y / m);
}

float Vec2::dot(const Vec2& v) const {
    return x * v.x + y * v.y;
}

float Vec2::cross(const Vec2& v) const {
    return x * v.y - y * v.x;
}

Vec2 Vec2::add(const Vec2& v) const {
    return Vec2(x + v.x, y + v.y);
}

float Vec2::distance(const Vec2& a, const Vec2& b) {
    return (a - b).getMag();
}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator-() const {
    return Vec2(-x, -y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
    return Vec2(x / scalar, y / scalar);
}

Vec2 operator*(float scalar, const Vec2& vec) {
    return Vec2(vec.x * scalar, vec.y * scalar);
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2 Vec2::random2d() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> real_distrib(-1.0f, 1.0f);
    return Vec2(real_distrib(gen), real_distrib(gen));
}

}
