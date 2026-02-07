#pragma once

namespace physix2d {

// A tiny 2D vector. Dumb data with the math it needs -- no more, no less.
class Vec2 {
public:
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x_, float y_);

    // --- magnitude helpers ---
    float lengthSq() const;          // x*x + y*y (cheap, no sqrt)
    float getMag() const;            // sqrt(lengthSq())
    Vec2  normalized() const;        // unit vector (zero-safe)

    // --- products ---
    float dot(const Vec2& v) const;  // scalar dot product
    float cross(const Vec2& v) const;// 2D scalar "cross" (z of 3D cross)
    Vec2  add(const Vec2& v) const;  // kept for backwards-compat

    static float distance(const Vec2& a, const Vec2& b);
    static Vec2 random2d();          // random vector, components in [-1, 1]

    // --- operators (all const-correct) ---
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator-() const;          // unary negate
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;
    friend Vec2 operator*(float scalar, const Vec2& vec);

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float scalar);
};

}
