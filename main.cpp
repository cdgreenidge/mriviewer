#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include <string>
#include "image.h"
#include "mri.h"
#include "model.h"
#include "utils.h"
#include "windows.h"

void setColorTheme();

void setColorTheme() {
    Fl::background2(255, 255, 255);
	Fl::background(0, 0, 0);
	Fl::foreground(200, 200, 200);
}

int main(int argc, char **argv) {
    setColorTheme();

    std::string file = readBinFile("/Users/cdg4/Downloads/mri.bin");
    const float * const data = reinterpret_cast<const float *> (file.data());
    Mri mri(data, 61, 73, 61, 78);
    Model model(mri);

	Fl_Window *mainWindow = new Fl_Window(800, 600, "MRI Viewer");
	mainWindow->size_range(600, 450, 0, 0, 0, 0, true);
	mainWindow->resizable(mainWindow);

	Fl_Group *coronalGroup = new Fl_Group(6, 21, 391, 276, "Coronal");
	coronalGroup->labelfont(FL_HELVETICA_BOLD);
	CoronalWindow *coronal = new CoronalWindow(6, 21, 391, 276, "Coronal", mri,
                                               model);
	coronalGroup->end();

	Fl_Group *sagittalGroup = new Fl_Group(402, 21, 391, 276, "Sagittal");
	sagittalGroup->labelfont(FL_HELVETICA_BOLD);
	SagittalWindow *sagittal = new SagittalWindow(402, 21, 391, 276,
                                                  "Sagittal", mri, model);
	sagittalGroup->end();

	Fl_Group *axialGroup = new Fl_Group(6, 318, 391, 276, "Axial");
	axialGroup->labelfont(FL_HELVETICA_BOLD);
	AxialWindow *axial = new AxialWindow(6, 318, 391, 276, "Axial",
                                         mri, model);
	axialGroup->end();

	mainWindow->end();
	mainWindow->show(argc, argv);

	return Fl::run();
}
