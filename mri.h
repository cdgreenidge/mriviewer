/* Required headers:
 * - boost/multi_array.hpp
 * - cstddef
 * - model.h
 * - image.h
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


struct Slice {
    Plane plane;
    size_t index;
};


class Mri {

    public:
        explicit Mri(const float * const data, const size_t x, const size_t y,
                     const size_t z, const size_t t);
        size_t width(const Plane plane) const;
        size_t height(const Plane plane) const;
        size_t numSlices(const Plane plane) const;
        size_t numVolumes() const;
        void fillImage(Image &image, Slice slice, size_t t) const;
        float max() const;

    private:
        Array4 data_;
        const size_t x_;
        const size_t y_;
        const size_t z_;
        const size_t t_;
        const float max_;

        Array2 subset(const Slice slice, const size_t t) const;
        float findMax(const Array4 arr) const;
};
