#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Multiline_Output.H>
#include <boost/multi_array.hpp>
#include <string>
#include <iostream>
#include "image.h"
#include "mri.h"
#include "model.h"
#include "utils.h"
#include "windows.h"
#include "main.h"

void setColorTheme();

void setColorTheme() {
  Fl::background2(0, 0, 0);
  Fl::background(0, 0, 0);
  Fl::foreground(200, 200, 200);
}

MainWindow::MainWindow(const Mri &mri, Model &model)
    : Fl_Window(800, 600, "MRI Viewer"),
      model_(model),
      coronalGroup_(new Fl_Group(6, 21, 391, 276, "Coronal")),
      sagittalGroup_(new Fl_Group(402, 21, 391, 276, "Sagittal")),
      axialGroup_(new Fl_Group(6, 318, 391, 276, "Axial")),
      coronal_(new CoronalWindow(6, 21, 391, 276, "Coronal", mri, model)),
      sagittal_(new SagittalWindow(402, 21, 391, 276, "Sagittal", mri, model)),
      axial_(new AxialWindow(6, 318, 391, 276, "Axial", mri, model)),
      scroller_(new TimeScrollbar(440, 318, 320, 20, mri, model)),
      position_(new Fl_Multiline_Output(440, 368, 100, 100, "")) {
  size_range(600, 450, 0, 0, 0, 0, true);
  resizable(this);
  add(coronalGroup_);
  coronalGroup_->labelfont(FL_HELVETICA_BOLD);
  coronalGroup_->add(coronal_);
  add(sagittalGroup_);
  sagittalGroup_->labelfont(FL_HELVETICA_BOLD);
  sagittalGroup_->add(sagittal_);
  add(axialGroup_);
  axialGroup_->labelfont(FL_HELVETICA_BOLD);
  axialGroup_->add(axial_);
  add(scroller_);
  scroller_->type(FL_HORIZONTAL);
  add(position_);
  position_->cursor_color(FL_BLACK);
  position_->box(FL_FLAT_BOX);
  updatePosition();

  if (mri.numVolumes() == 1) {
    // This is not a functional MRI, so disable the time slider
    scroller_->deactivate();
  }
}

void MainWindow::redrawMri() {
  coronal_->redraw();
  sagittal_->redraw();
  axial_->redraw();
  updatePosition();
  return;
}

int MainWindow::handle(int event) {
  char key;
  switch (event) {
    case FL_KEYBOARD:
      key = static_cast<char>(Fl::event_key());
      switch (key) {
        case '=':
        /* Fallthrough */
        case '+':
          model_.zoomIn();
          break;
        case '_':
        /* Fallthrough */
        case '-':
          model_.zoomOut();
          break;
      }
      break;
    default:
      return Fl_Window::handle(event);
  }
  redrawMri();
  return 1;
}

void MainWindow::updatePosition() {
  sprintf(positionBuffer_, "Position:\n\nX:\t%lu\nY:\t%lu\nZ:\t%lu\nT:\t%lu",
          model_.x() + 1, model_.y() + 1, model_.z() + 1, model_.t() + 1);
  position_->value(positionBuffer_);
  return;
}

static void updateTimeCb(Fl_Widget *widget, void *) {
  TimeScrollbar *timeScrollbar = static_cast<TimeScrollbar *>(widget);
  timeScrollbar->updateTime();
  return;
}

TimeScrollbar::TimeScrollbar(int X, int Y, int W, int H, Mri mri, Model &model)
    : Fl_Scrollbar(X, Y, W, H, "Time"), mri_(mri), model_(model) {
  bounds(0, mri.numVolumes());
  value(0);
  step(1);
  linesize(1);
  callback(updateTimeCb);
  return;
}

void TimeScrollbar::updateTime() {
  model_.setTime(value());
  (static_cast<MainWindow *>(parent()))->redrawMri();
  return;
}

int main(int argc, char **argv) {
  setColorTheme();

  std::string file = readBinFile("/Users/cdg4/Dropbox/research/mri.bin");
  const float *const data = reinterpret_cast<const float *>(file.data());
  Mri mri(data, 61, 73, 61, 78);
  Model model(mri);

  MainWindow *mainWindow = new MainWindow(mri, model);
  mainWindow->show(argc, argv);

  return Fl::run();
}
