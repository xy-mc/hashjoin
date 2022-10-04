/// A wrapper file ensuring 64bit key size.

#ifndef GENERATOR64_HPP
#define GENERATOR64_HPP

//#define KEY_8B
#include "constants.h"
#include "generator.h"

namespace eth_hashjoin {
#if defined(KEY_8B)
static_assert(sizeof(intkey_t) == 8);
static_assert(sizeof(value_t) == 8);
#else
static_assert(sizeof(intkey_t) == 4);
static_assert(sizeof(value_t) == 4);
#endif
} // namespace eth_hashjoin
#endif // GENERATOR64_HPP
