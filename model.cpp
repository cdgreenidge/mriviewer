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

size_t Model::x() const noexcept {
    return x_;
}

size_t Model::y() const noexcept {
    return y_;
}

size_t Model::z() const noexcept {
    return z_;
}

size_t Model::t() const noexcept {
    return t_;
}

float Model::scale() const noexcept {
    return scale_;
}


float checkScale(const float scale) {
    if (scale <= 0.0f || 1.0f < scale) {
        throw std::out_of_range("scale must be in (0, 1]");
    }
    return scale;
}
