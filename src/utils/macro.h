#ifndef MACVELOX_MACRO_H
#define MACVELOX_MACRO_H

#include <time.h>
#include <chrono>

#include "exception.h"

#define TIME_NANO_DIFF(finish, start) \
  (finish.tv_sec - start.tv_sec) * 1000000000 + (finish.tv_nsec - start.tv_nsec)

#define TIME_MICRO_OR_RAISE(time, expr)                                        \
  do {                                                                         \
    auto start = std::chrono::steady_clock::now();                             \
    auto __s = (expr);                                                         \
    if (!__s.ok()) {                                                           \
      return __s;                                                              \
    }                                                                          \
    auto end = std::chrono::steady_clock::now();                               \
    time += std::chrono::duration_cast<std::chrono::microseconds>(end - start) \
                .count();                                                      \
  } while (false);

#define TIME_MICRO_OR_THROW(time, expr)                                        \
  do {                                                                         \
    auto start = std::chrono::steady_clock::now();                             \
    auto __s = (expr);                                                         \
    if (!__s.ok()) {                                                           \
      throw PetrelException(__s.message());                                    \
    }                                                                          \
    auto end = std::chrono::steady_clock::now();                               \
    time += std::chrono::duration_cast<std::chrono::microseconds>(end - start) \
                .count();                                                      \
  } while (false);

#define TIME_MICRO(time, res, expr)                                            \
  do {                                                                         \
    auto start = std::chrono::steady_clock::now();                             \
    res = (expr);                                                              \
    auto end = std::chrono::steady_clock::now();                               \
    time += std::chrono::duration_cast<std::chrono::microseconds>(end - start) \
                .count();                                                      \
  } while (false);

#define TIME_NANO_OR_RAISE(time, expr)                                        \
  do {                                                                        \
    auto start = std::chrono::steady_clock::now();                            \
    auto __s = (expr);                                                        \
    if (!__s.ok()) {                                                          \
      return __s;                                                             \
    }                                                                         \
    auto end = std::chrono::steady_clock::now();                              \
    time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) \
                .count();                                                     \
  } while (false);

#define TIME_NANO_OR_THROW(time, expr)                                        \
  do {                                                                        \
    auto start = std::chrono::steady_clock::now();                            \
    auto __s = (expr);                                                        \
    if (!__s.ok()) {                                                          \
      throw PetrelException(__s.message());                                   \
    }                                                                         \
    auto end = std::chrono::steady_clock::now();                              \
    time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) \
                .count();                                                     \
  } while (false);

#define VECTOR_PRINT(v, name)          \
  std::cout << "[" << name << "]:";    \
  for (int i = 0; i < v.size(); i++) { \
    if (i != v.size() - 1)             \
      std::cout << v[i] << ",";        \
    else                               \
      std::cout << v[i];               \
  }                                    \
  std::cout << std::endl;

#define THROW_NOT_OK(expr)                  \
  do {                                      \
    auto __s = (expr);                      \
    if (!__s.ok()) {                        \
      throw PetrelException(__s.message()); \
    }                                       \
  } while (false);

#define TIME_TO_STRING(time) \
  (time > 10000 ? time / 1000 : time) << (time > 10000 ? " ms" : " us")

#define TIME_NANO_TO_STRING(time)                                \
  (time > 1e7 ? time / 1e6 : ((time > 1e4) ? time / 1e3 : time)) \
      << (time > 1e7 ? "ms" : (time > 1e4 ? "us" : "ns"))

#endif //MACVELOX_MACRO_H
