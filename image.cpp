#include <FL/gl.H>
#include <cstddef>
#include "image.h"

Image::Image(const size_t width, const size_t height) :
    data_(new GLfloat[width * height]),
    width_(width),
    height_(height)
{ }

Image::~Image() {
    delete[] data_;
}

size_t Image::width() const noexcept {
    return width_;
}

size_t Image::height() const noexcept {
    return height_;
}

GLfloat *Image::data() const noexcept {
    return data_;
}

