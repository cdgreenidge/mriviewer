#include <cstddef>
#include <Fl/gl.h>
#include "slice.h"

Slice::Slice(const ImageBuffer image, const Crosshair crosshair,
			 const Labels labels) :
	image_(image),
	crosshair_(crosshair),
	labels_(labels)
{ }
