#include <iostream>
#include <vector>

#include "xsimd/xsimd.hpp"

namespace {

template<class T>
void printSimdInfo() {
  constexpr auto simd_type_size = xsimd::simd_type<T>::size;
  auto arch_type = typeid(typename xsimd::simd_type<T>::arch_type).name();
  auto register_type = typeid(typename xsimd::simd_type<T>::register_type).name();
  auto simd_register = typeid(typename xsimd::simd_type<T>::simd_register).name();

  std::cout << "arch_type " << arch_type << std::endl;
  std::cout << "simd_type_size " << simd_type_size << std::endl;
  std::cout << "register_type " << register_type << std::endl;
  std::cout << "simd_register " << simd_register << std::endl;
}

/// Is xsimd::default_arch extracted using traits during compilation time?
/// Can we prove it through get type info during compilation.
/// Using the following codes and get compilation error to get the type info,
/// "error: implicit instantiation of undefined template '(anonymous
/// namespace)::WhichType<xsimd::neon64>'"
///
/// template <typename...> struct WhichType;
/// template<typename Arch = xsimd::default_arch>
/// void foo() {
///  WhichType<xsimd::default_arch>{};
/// }

template <typename Arch = xsimd::default_arch>
void getArch() {
  std::cout << "arch_type is " << typeid(Arch).name() << std::endl;
}
}

int main(int argc, char* argv[]) {
  std::cout << "Get simd size automatically.\n";
  std::cout << "Get simd info of int8_t\n";
  printSimdInfo<int8_t>();
  std::cout << "Get simd info of int16_t\n";
  printSimdInfo<int16_t>();
  std::cout << "Get simd info of int32_t\n";
  printSimdInfo<int32_t>();
  std::cout << "Get simd info of int64_t\n";
  printSimdInfo<int64_t>();
  std::cout << "Get simd info of double\n";
  printSimdInfo<double>();
  // type 'simd_type<bool>' (aka 'bool') cannot be used prior to '::' because it has no members
  // printSimdInfo<bool>();
  getArch();

  return 0;
}
