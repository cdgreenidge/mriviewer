#include <cstddef>
#include <stdexcept>
#include "model.h"

Model::Model(size_t x, size_t y, size_t z, size_t t, float scale) :
    x_(x),
    y_(y),
    z_(z),
    t_(t),
    scale_(checkScale(scale))
{ }

float Model::scale() const noexcept {
    return scale_;
}


float checkScale(const float scale) {
    if (scale <= 0.0f || 1.0f < scale) {
        throw std::out_of_range("scale must be in (0, 1]");
    }
    return scale;
}
