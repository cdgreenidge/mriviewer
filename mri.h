/* Required headers:
 * - cstddef
 * - boost/multi_array.hpp
 */

typedef boost::const_multi_array_ref<float, 4> Array4;
typedef Array4::array_view<2>::type Array2;
typedef boost::multi_array_types::index Index;
typedef boost::multi_array_types::index_range Range;

class Mri {

public:
	Mri(const float * const data, const size_t x, const size_t y,
		const size_t z, const size_t t);

private:
	Array4 data_;
};
