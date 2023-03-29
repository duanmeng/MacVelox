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

  return 0;
}
