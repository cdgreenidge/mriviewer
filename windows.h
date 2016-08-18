/* Required headers:
 * - FL/Fl.H
 * - FL/gl.H
 * - FL/Fl_Gl_Window.H
 * - boost/multi_array.hpp
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
              const Plane plane, const Labels labels);

protected:
    Mri mri_;
    Model model_;
    Plane plane_;
    Labels labels_;
};


class CoronalWindow : MriWindow {

public:
    CoronalWindow(const int x, const int y, const int w, const int n,
                  const char* const i, const Mri mri, const Model model);
};


class SagittalWindow : MriWindow {

public:
    SagittalWindow(const int x, const int y, const int w, const int n,
                   const char* const i, const Mri mri, const Model model);
};


class AxialWindow : MriWindow {

public:
    AxialWindow(const int x, const int y, const int w, const int n,
                const char* const i, const Mri mri, const Model model);
};
