#include <iostream>
#include <vector>

#include "xsimd/xsimd.hpp"

namespace xs = xsimd;
using vector_type = std::vector<double, xsimd::aligned_allocator<double>>;

namespace {
void mean(const vector_type& a, const vector_type& b, vector_type& res) {
  std::size_t size = a.size();
  constexpr std::size_t simd_size = xsimd::simd_type<double>::size;
  std::size_t vec_size = size - size % simd_size;

  for (std::size_t i = 0; i < vec_size; i += simd_size) {
    auto ba = xs::load_aligned(&a[i]);
    auto bb = xs::load_aligned(&b[i]);
    auto bres = (ba + bb) / 2;
    bres.store_aligned(&res[i]);
  }

  for (std::size_t i = vec_size; i < size; ++i) {
    res[i] = (a[i] + b[i]) / 2;
  }
}

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
  constexpr std::size_t simd_size = xsimd::simd_type<double>::size;
  xs::batch<double, xs::neon64> a{1.0, 2.0};
  xs::batch<double, xs::neon64> b{3.0, 4.0};
  std::cout << (a + b) / 2 << std::endl;

  std::cout << "Get simd size automatically.\n";

  auto a1 = vector_type{1.0, 2.0, 3.0, 4.0, 5.0};
  auto a2 = vector_type{1.0, 2.0, 3.0, 4.0, 5.0};
  vector_type a3(5, 0.0);
  mean(a1, a2, a3);

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