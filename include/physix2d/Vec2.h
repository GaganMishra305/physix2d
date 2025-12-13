#pragma once

namespace physix2d {

class Vec2{
public:
    float x = 0.0f;
    float y = 0.0f;
    float getX() const;
    float getY() const;
    float getMag() const;

    // custom operations
    Vec2(float x_, float y_);
    int dot(Vec2 v);
    Vec2 cross(Vec2 v);
    Vec2 add(Vec2 v);
    static Vec2 random2d(); // returns a random 2d vector

    // operator overloads
    Vec2 operator+(const Vec2& other);
    Vec2 operator-(const Vec2& other);
    Vec2 operator*(float scalar);
    friend Vec2 operator*(float scalar, const Vec2& vec);
    Vec2& operator=(const Vec2& other);
};

}