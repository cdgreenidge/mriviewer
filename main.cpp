#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include <string>
#include "image.h"
#include "model.h"
#include "mri.h"
#include "utils.h"
#include "windows.h"

void setColorTheme();

void setColorTheme() {
    Fl::background2(255, 255, 255);
	Fl::background(0, 0, 0);
	Fl::foreground(200, 200, 200);
}

int main() {
    setColorTheme();

    std::string file = readBinFile("/Users/cdg4/Downloads/mri.bin");
    const float * const data = reinterpret_cast<const float *> (file.data());
    Mri mri(data, 61, 73, 61, 78);

    return 0;
}
