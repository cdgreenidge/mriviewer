#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include "image.h"
#include "mri.h"
#include "model.h"
#include "windows.h"

MriWindow::MriWindow(const int x, const int y, const int w, const int h,
                     const char * const i, const Mri mri, const Model model,
                     const Plane plane, const Labels labels) :
    Fl_Gl_Window(x, y, w, h, i),
    mri_(mri),
    model_(model),
    plane_(plane),
    image_(mri.width(plane), mri.height(plane)),
    labels_(labels) {
    mri.fillImage(image_, model.slice(plane), model.t());
}


CoronalWindow::CoronalWindow(const int x, const int y, const int w,
                             const int h, const char* const i, const Mri mri,
                             const Model model) :
    MriWindow(x, y, w, h, i, mri, model, CORONAL, Labels {"L", "S", "R", "I"})
{ }


SagittalWindow::SagittalWindow(const int x, const int y, const int w,
                               const int h, const char* const i, const Mri mri,
                               const Model model) :
    MriWindow(x, y, w, h, i, mri, model, SAGITTAL, Labels {"A", "S", "P", "I"})
{ }


AxialWindow::AxialWindow(const int x, const int y, const int w,
                         const int h, const char* const i, const Mri mri,
                         const Model model) :
    MriWindow(x, y, w, h, i, mri, model, AXIAL, Labels {"L", "A", "R", "P"})
{ }


void MriWindow::draw() {
    return;
}
