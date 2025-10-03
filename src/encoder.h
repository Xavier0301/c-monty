#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "math.h"

#include "types.h"

void encode_integer(u8* output, u32 input, pair_u32 range, u32 num_bits, u32 num_active_bits);

#endif // ENCODER_H
