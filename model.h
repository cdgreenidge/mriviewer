/* Required headers:
 * - FL/gl.H
 * - boost/multi_array.hpp
 * - cstddef
 * - model.h
 * - image.h
 */

struct Crosshair {
  size_t x;
  size_t y;
};

class Model {
 public:
  explicit Model(size_t x, size_t y, size_t z, size_t t, float scale);
  explicit Model(Mri mri);
  Slice slice(Plane plane) const noexcept;
  Crosshair crosshair(Plane plane) const noexcept;
  size_t t() const noexcept;
  float scale() const noexcept;

  // Callbacks for GUI event handling
  void updatePush(const float textureX, const float textureY);

 private:
  size_t x_;
  size_t y_;
  size_t z_;
  size_t t_;
  float scale_;

  float checkScale(const float scale) const;
};
