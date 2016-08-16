#include <boost/multi_array.hpp>
#include "mri.h"

Mri::Mri(const float *data, const size_t x, const size_t y,
		 const size_t z, const size_t t) :
	data_(data, boost::extents[x][y][z][t], boost::fortran_storage_order())
{ }
