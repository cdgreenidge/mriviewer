#include <boost/multi_array.hpp>
#include <stdexcept>
#include "model.h"
#include "mri.h"

Mri::Mri(const float *data, const size_t x, const size_t y,
         const size_t z, const size_t t) :
    data_(data, boost::extents[x][y][z][t], boost::fortran_storage_order()),
    x_(x),
    y_(y),
    z_(z),
    t_(t),
    max_(findMax(data_))
{ }

size_t Mri::width(Plane plane) const {
    switch (plane) {
        case CORONAL:
            return x_;
        case SAGITTAL:
            return y_;
        case AXIAL:
            return x_;
    }
}

size_t Mri::height(Plane plane) const {
    switch (plane) {
        case CORONAL:
            return z_;
        case SAGITTAL:
            return z_;
        case AXIAL:
            return y_;
    }
}

size_t Mri::numSlices(Plane plane) const {
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

Array2 Mri::subset(Slice slice, size_t t) const {
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
