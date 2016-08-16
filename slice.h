/* Required headers:
 *  - cstddef
 *  - Fl/gl.h
 */

struct ImageBuffer {
	const GLfloat * const data;
	const size_t width;
	const size_t height;
};

struct Crosshair {
	size_t x;
	size_t y;
};

struct Labels {
	const char * const right = "r";
	const char * const top = "t";
	const char * const left = "l";
	const char * const bottom = "b";
};

class Slice {

public:
	explicit Slice(const ImageBuffer image, const Crosshair crosshair,
				   const Labels labels);
	void draw() const;
	void setCrosshair(const Crosshair crosshair);

private:
	const ImageBuffer image_;
	Crosshair crosshair_;
	const Labels labels_;
};
