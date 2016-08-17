/* Required headers:
 * - cstddef
 */

enum Plane {
    CORONAL,
    SAGITTAL,
    AXIAL
};

struct Slice {
    Plane plane;
    size_t index;
};

struct Crosshair {
    size_t x;
    size_t y;
};

class Model {

public:
    explicit Model(size_t x, size_t y, size_t z, size_t t, float scale);
    float scale() const noexcept;

private:
    size_t x_;
    size_t y_;
    size_t z_;
    size_t t_;
    float scale_;
};

float checkScale(const float scale);
