/* Required headers:
 * - cstddef
 * - boost/multi_array.hpp
 */

typedef boost::const_multi_array_ref<float, 4> Array4;
typedef Array4::const_array_view<2>::type Array2;
typedef boost::multi_array_types::index Index;
typedef boost::multi_array_types::index_range Range;

enum Plane {
    CORONAL,
    SAGITTAL,
    AXIAL
};

class Mri {

public:
    explicit Mri(const float * const data, const size_t x, const size_t y,
                 const size_t z, const size_t t);
    size_t width(Plane plane) const;
    size_t height(Plane plane) const;
    size_t numSlices(Plane plane) const;
    Array2 slice(Plane plane, size_t i, size_t t) const;
    float max() const;

private:
    Array4 data_;
    const size_t x_;
    const size_t y_;
    const size_t z_;
    const size_t t_;
    const float max_;

    Array2 coronalSlice(const size_t y, const size_t t) const;
    Array2 sagittalSlice(const size_t x, const size_t t) const;
    Array2 axialSlice(const size_t z, const size_t t) const;
    float findMax(const Array4 arr) const;
};
