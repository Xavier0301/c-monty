#ifndef INTERFACES_H
#define INTERFACES_H

#include "types.h"
#include "location.h"

// Defines the number of fractional bits in the output curvature values.
// A value of 8 means the integer result should be divided by 2^8 = 256.
#define CURVATURE_FRACTIONAL_BITS 8
static const u32 PC1_IS_PC2_THRESHOLD_FP = (1 << CURVATURE_FRACTIONAL_BITS) - 1;

typedef struct features_t_ {
    u32 value;
    u8 min_depth;
    u8 max_depth;
    u8 mean_depth;

    i32 principal_curvature_1_fp; // fixed-point with CURVATURE_FRACTIONAL_BITS bits
    i32 principal_curvature_2_fp; // fixed-point with CURVATURE_FRACTIONAL_BITS bits

    int pose_fully_defined;
} features_t;

typedef struct pose_t_ {
    vec3d point_normal;
    vec3d curvature_direction_1;
    vec3d curvature_direction_2;

    int pose_fully_defined;
} pose_t;

#endif
