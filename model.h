/* Required headers:
 * - cstddef
 */

enum Plane {
    CORONAL,
    SAGITTAL,
    AXIAL
};

class Model {

public:
    explicit Model(size_t x, size_t y, size_t z, size_t t, float scale);
    size_t x() const noexcept;
    size_t y() const noexcept;
    size_t z() const noexcept;
    size_t t() const noexcept;
    float scale() const noexcept;

private:
    size_t x_;
    size_t y_;
    size_t z_;
    size_t t_;
    float scale_;
};

float checkScale(const float scale);
