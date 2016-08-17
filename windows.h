/* Required headers:
 * - FL/Fl.H
 * - FL/gl.H
 * - FL/Fl_Gl_Window.H
 * - image.h
 * - slice.h
 * - model.h
 * - mri.h
 */

struct Labels {
    const char * const right;
    const char * const top;
    const char * const left;
    const char * const bottom;
};

class MriWindow : public Fl_Gl_Window {

public:
    MriWindow(const int x, const int y, const int w, const int h,
              const char * const i, const Mri mri, const Model model,
              const Labels labels);

protected:
    Mri mri_;
    Model model_;
    Labels labels_;
};
