#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "algorithms.h"

#include "tensor.h"
#include "types.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/************* UNIFORM ***********/
f32 unif_rand_f32(f32 max); // inclusive
u32 unif_rand_u32(u32 max); // inclusive

f32 unif_rand_range_f32(f32 min, f32 max); // inclusive
u32 unif_rand_range_u32(u32 min, u32 max); // inclusive

#define SHUFFLE_ARRAY_DEFINITION(symbol) \
    void shuffle_array_##symbol(symbol* array, u32 length)

SHUFFLE_ARRAY_DEFINITION(u8);
SHUFFLE_ARRAY_DEFINITION(u16);
SHUFFLE_ARRAY_DEFINITION(u32);

/************* GAUSSIAN ***********/
// Returns a random number sampled from N(0,1)
double gauss_rand();

// Inverse of the error function erf
double erf_inv(double x);

// Quantile of Gaussian distribution  
double gauss_inv(double p);

#endif // DISTRIBUTIONS_H
