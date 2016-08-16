#include <boost/multi_array.hpp>
#include <stdexcept>
#include "mri.h"

Mri::Mri(const float *data, const size_t x, const size_t y,
         const size_t z, const size_t t) :
    data_(data, boost::extents[x][y][z][t], boost::fortran_storage_order()),
    x_(x),
    y_(y),
    z_(z),
    t_(t)
{ }

Array2 Mri::coronalSlice(const size_t y, const size_t t) const {
    if (y > y_) {
        throw std::out_of_range("y out of range");
    } else if (t > t_) {
        throw std::out_of_range("t out of range");
    }
    return data_[boost::indices[Range()][y][Range()][t]];
}

Array2 Mri::sagittalSlice(const size_t x, const size_t t) const {
    if (x > x_) {
        throw std::out_of_range("x out of range");
    } else if (t > t_) {
        throw std::out_of_range("t out of range");
    }
    return data_[boost::indices[x][Range()][Range()][t]];
}

Array2 Mri::axialSlice(const size_t z, const size_t t) const {
    if (z > z_) {
        throw std::out_of_range("z out of range");
    } else if (t > t_) {
        throw std::out_of_range("t out of range");
    }
    return data_[boost::indices[Range()][Range()][z][t]];
}


float max(const Array4 &arr) {
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
