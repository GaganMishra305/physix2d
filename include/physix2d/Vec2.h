#pragma once

namespace physix2d {

class Vec2{
private:
    float x = 0.0f, y = 0.0f;

public:
    float getX() const;
    float getY() const;

    Vec2(float x_, float y_);
    int dot(Vec2 v);
    Vec2 cross(Vec2 v);
    Vec2 add(Vec2 v);
};

}