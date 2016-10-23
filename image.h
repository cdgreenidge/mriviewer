/* Required headers:
 * - FL/gl.H
 * - cstddef
 */

class Image {
 public:
  explicit Image(const size_t width, const size_t height);
  ~Image();
  GLfloat *data() const noexcept;
  size_t width() const noexcept;
  size_t height() const noexcept;

 private:
  GLfloat *const data_;
  const size_t width_;
  const size_t height_;
};
