#include <FL/gl.H>
#include <boost/multi_array.hpp>
#include <cstddef>
#include <stdexcept>
#include "image.h"
#include "mri.h"
#include "model.h"

Model::Model(size_t x, size_t y, size_t z, size_t t, float scale) :
    x_(x),
    y_(y),
    z_(z),
    t_(t),
    scale_(checkScale(scale))
{ }

Model::Model(Mri mri) :
    x_(mri.numSlices(SAGITTAL) / 2),
    y_(mri.numSlices(CORONAL) / 2),
    z_(mri.numSlices(AXIAL) / 2),
    t_(mri.numVolumes()),
    scale_(1.0f)
{ }

Slice Model::slice(Plane plane) const noexcept {
    switch (plane) {
        case CORONAL:
            return Slice {plane, y_};
        case SAGITTAL:
            return Slice {plane, x_};
        case AXIAL:
            return Slice {plane, z_};
    }
}

Crosshair Model::crosshair(Plane plane) const noexcept {
    switch (plane) {
        case CORONAL:
            return Crosshair {x_, z_};
        case SAGITTAL:
            return Crosshair {y_, z_};
        case AXIAL:
            return Crosshair {x_, y_};
    }
}

size_t Model::t() const noexcept {
    return t_;
}

float Model::scale() const noexcept {
    return scale_;
}

float Model::checkScale(const float scale) const {
    if (scale <= 0.0f || 1.0f < scale) {
        throw std::out_of_range("scale must be in (0, 1]");
    }
    return scale;
}
