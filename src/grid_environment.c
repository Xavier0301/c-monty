#include "grid_environment.h"

#include "assertf.h"
#include "distributions.h"

void init_grid_env(grid_t* env, u32 rows, u32 cols) {
    matrix_u8_init(&env->depths, rows, cols);
    matrix_u32_init(&env->values, rows, cols);

    env->rows = rows;
    env->cols = cols;
}

void populate_grid_env_random(grid_t* env) {
    for(u32 i = 0; i < env->rows; ++i) {
        for(u32 j = 0; j < env->rows; ++j) {
            MAT(env->depths, i, j) = unif_rand_range_u32(0, 4);
            MAT(env->values, i, j) = unif_rand_range_u32(10, 50);
        }
    }
}

bounds_t get_bounds(u32 env_size_x, u32 env_size_y, u32 patch_size_x, u32 patch_size_y) {
    return (bounds_t) {
        .min_x = patch_size_x / 2,
        .max_x = env_size_x - patch_size_x / 2,

        .min_y = patch_size_y / 2,
        .max_y = env_size_y - patch_size_y / 2
    };
}


/**
 * @brief 
 * 
 * @param patch pre-allocated! (of shape (patch_sidelen, patch_sidelen))
 * @param env 
 * @param location 
 * @param patch_radius 
 */
void extract_patch(grid_t* patch, grid_t* env, vec2d location, u32 patch_sidelen) {
    assertf(patch_sidelen == patch->rows && patch_sidelen== patch->cols, 
        "mismatch between patch_radius and actually allocated patch shape");
    assertf(patch_sidelen % 2 != 0, "patch cannot be of even sidelength");

    u32 patch_radius = patch_sidelen / 2;
    u32 start_row = location.x - patch_radius;
    u32 start_col = location.y - patch_radius;
    
    for(u32 row = 0; row < patch->rows; ++row) {
        for(u32 col = 0; col < patch->cols; ++col) {
            MAT(patch->values, row, col) = MAT(env->values, start_row + row, start_col + col);
            MAT(patch->depths, row, col) = MAT(env->depths, start_row + row, start_col + col);
        }
    }
}


void print_grid(grid_t* env) {
    printf("depths:\n");
    MATRIX_PRINT(&env->depths, env->rows, env->cols);
    printf("values:\n");
    MATRIX_PRINT(&env->values, env->rows, env->cols);
}
