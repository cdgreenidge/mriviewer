/* Required headers:
 * - FL/Fl.H
 * - FL/gl.H
 * - FL/Fl_Gl_Window.H
 * - boost/multi_array.hpp
 * - slice.h
 * - mri.h
 */

class MriWindow : public Fl_Gl_Window {

public:
    MriWindow(const int x, const int y, const int w, const int h,
              const char * const label, const Mri mri, const Slice slice);

private:
    Mri mri_;
    Slice slice_;
};
