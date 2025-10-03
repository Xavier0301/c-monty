#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define PAIR_TYPE_(symbol) pair_##symbol##_
#define PAIR_TYPE(symbol) pair_##symbol

#define DEFINE_PAIR_STRUCT(symbol) \
    typedef struct PAIR_TYPE_(symbol) { \
        symbol first; \
        symbol second; \
    } PAIR_TYPE(symbol)

DEFINE_PAIR_STRUCT(u8);
DEFINE_PAIR_STRUCT(u16);
DEFINE_PAIR_STRUCT(u32);

#define INDEX2D_TYPE_(symbol) index2d_##symbol##_
#define INDEX2D_TYPE(symbol) index2d_##symbol

#define DEFINE_INDEX2D_STRUCT(symbol) \
    typedef struct INDEX2D_TYPE_(symbol) { \
        symbol x; \
        symbol y; \
    } INDEX2D_TYPE(symbol)

DEFINE_INDEX2D_STRUCT(u8);
DEFINE_INDEX2D_STRUCT(u16);
DEFINE_INDEX2D_STRUCT(u32);

#endif // TYPES_H
