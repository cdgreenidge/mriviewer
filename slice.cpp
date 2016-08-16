#include <cstddef>
#include <Fl/gl.h>
#include "slice.h"

Slice::Slice(const ImageBuffer image, const Crosshair crosshair,
             const Labels labels) :
    image_(image),
    crosshair_(crosshair),
    labels_(labels)
{ }

void Slice::draw() const {
    /* TODO: fill out this method. Maybe move into a separate file? */
    (void) image_;
    return;
}

void Slice::setCrosshair(const Crosshair crosshair) {
    crosshair_ = crosshair;
}
