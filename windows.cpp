#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include "image.h"
#include "model.h"
#include "mri.h"
#include "windows.h"

MriWindow::MriWindow(const int x, const int y, const int w, const int h,
                     const char * const i, const Mri mri, const Model model,
                     const Labels labels) :
    Fl_Gl_Window(x, y, w, h, i),
    mri_(mri),
    model_(model),
    labels_(labels)
{ }
