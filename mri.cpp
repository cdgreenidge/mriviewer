#include <boost/multi_array.hpp>
#include <FL/gl.H>
#include "image.h"
#include "mri.h"
#include "model.h"

Mri::Mri(const float *data, const size_t x, const size_t y,
         const size_t z, const size_t t) :
    data_(data, boost::extents[x][y][z][t], boost::fortran_storage_order()),
    x_(x),
    y_(y),
    z_(z),
    t_(t),
    max_(findMax(data_))
{ }

size_t Mri::width(const Plane plane) const {
    switch (plane) {
        case CORONAL:
            return x_;
        case SAGITTAL:
            return y_;
        case AXIAL:
            return x_;
    }
}

size_t Mri::height(const Plane plane) const {
    switch (plane) {
        case CORONAL:
            return z_;
        case SAGITTAL:
            return z_;
        case AXIAL:
            return y_;
    }
}

size_t Mri::numSlices(const Plane plane) const {
    switch (plane) {
        case CORONAL:
            return y_;
        case SAGITTAL:
            return x_;
        case AXIAL:
            return z_;
    }
}

size_t Mri::numVolumes() const {
    return t_;
}

void Mri::fillImage(Image &image, Slice slice, size_t t) const {
    size_t imWidth = image.width();
    size_t imHeight = image.height();
    if (imWidth != width(slice.plane)) {
        throw std::invalid_argument("image width not equal to slice width");
    } else if (imHeight != height(slice.plane)) {
        throw std::invalid_argument("image height not equal to slice height");
    }

    Array2 arr = subset(slice, t);
    GLfloat *data = image.data();
    for (size_t i = 0; i < imWidth; i++) {
        for (size_t j = 0; j < imHeight; j++) {
            data[i + j*imWidth] = arr[i][j] / max_;
        }
    }
    return;
}

Array2 Mri::subset(const Slice slice, const size_t t) const {
    switch (slice.plane) {
        case CORONAL:
            return data_[boost::indices[Range()][slice.index][Range()][t]];
        case SAGITTAL:
            return data_[boost::indices[slice.index][Range()][Range()][t]];
        case AXIAL:
            return data_[boost::indices[Range()][Range()][slice.index][t]];
    }
}

float Mri::max() const {
    return max_;
}

float Mri::findMax(const Array4 arr) const {
    float val = arr[0][0][0][0];
    const float * const begin = arr.origin();
    const float * const end = begin + arr.num_elements();
    for (const float *i = begin; i < end; i++) {
        if (*i > val) {
            val = *i;
        }
    }
    return val;
}
