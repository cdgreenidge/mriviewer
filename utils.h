/* Required header files:
 * - string
 */

std::string readBinFile(const char* const fname);

template <class T>
T clamp(T x, T min, T max) {
  return std::max<T>(min, std::min<T>(x, max));
}
