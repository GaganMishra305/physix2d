#pragma once

namespace physix2d {

class Force {
public:
    virtual ~Force() = default;
    virtual void apply(float dt) = 0;
};

}
