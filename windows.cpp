#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include "model.h"
#include "mri.h"
#include "windows.h"

MriWindow::MriWindow(const int x, const int y, const int w, const int h,
                     const char * const label, const Mri mri,
                     const Model model) :
    Fl_Gl_Window(x, y, w, h, label),
    mri_(mri),
    model_(model)
{ }
