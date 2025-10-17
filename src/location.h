#ifndef LOCATION_H
#define LOCATION_H

#include "types.h"

typedef struct vec2d_ {
    i32 x;
    i32 y;
} vec2d;

typedef struct vec3d_ {
    i32 x;
    i32 y;
    i32 z;
} vec3d;

inline static int is_vec2d_positive(vec2d v) {
    return v.x >= 0 && v.y >= 0;
}

inline static vec2d vec_divided_u32(vec2d v, u32 d) {
    return (vec2d) {
        .x = v.x / d,
        .y = v.y / d
    };
}

inline static vec2d vec_multiplied_u32(vec2d v, u32 d) {
    return (vec2d) {
        .x = v.x * d,
        .y = v.y * d
    };
}

#endif
