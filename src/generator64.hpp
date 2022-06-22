/// A wrapper file ensuring 64bit key size.

#ifndef GENERATOR64_HPP
#define GENERATOR64_HPP

#define KEY_8B
#include "generator.h"

static_assert(sizeof(intkey_t) == 8);
static_assert(sizeof(value_t) == 8);

#endif // GENERATOR64_HPP
