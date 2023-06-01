#ifndef SAYTINYSTL_EXCEPTDEF_H
#define SAYTINYSTL_EXCEPTDEF_H

#include <stdexcept>
#include <cassert>

namespace saystl {

#define SAYSTL_DEBUG(expr) \
  assert(expr)  

#define THROW_LENGTH_ERROR_IF(expr, what) \
  if ((expr)) throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what) \
  if ((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what) \
  if ((expr)) throw std::runtime_error(what)


}// !namespace saystl

#endif // !SAYTINYSTL_EXCEPTDEF_H


