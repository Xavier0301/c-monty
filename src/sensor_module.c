#include "sensor_module.h"

#include "assertf.h"

/**
 * @brief Get features and pose from the patch observed
 * 
 * @param features 
 * @param poses 
 * @param patch 
 * @param location 
 */
void sensor_module(features_t* features, pose_t* pose, grid_t patch, vec2d location) {
    // -- Pose --
    vec3d point_normal;
    get_point_normal_u8(&point_normal, patch.depths, location);

    i32 k1_fp, k2_fp;
    vec3d dir1, dir2;
    get_principal_curvatures_u8(&k1_fp, &k2_fp, &dir1, &dir2, patch.depths, location);

    pose->point_normal = point_normal;
    pose->curvature_direction_1 = dir1;
    pose->curvature_direction_2 = dir2;
    pose->pose_fully_defined = (u32) abs((i32) k1_fp - (i32) k2_fp) > PC1_IS_PC2_THRESHOLD_FP;

    // -- Features --
    features->value = MAT(patch.values, location.x, location.y);
    features->principal_curvature_1_fp = k1_fp;
    features->principal_curvature_2_fp = k2_fp;

    features->min_depth = mat_u8_min(patch.depths);
    features->max_depth = mat_u8_max(patch.depths);
    features->mean_depth = mat_u8_mean(patch.depths);

    features->pose_fully_defined = pose->pose_fully_defined;
}

/**
 * @brief Get the point normal object
 * 
 * @param point_normal 
 * @param depths 
 * @param loc Assumed to be not on the edge of 'depths'!
 */
void get_point_normal_u8(vec3d* point_normal, mat_u8 depths, vec2d location) {
    if(depths.rows <= 1) {
        point_normal->x = 0;
        point_normal->y = 0;
        point_normal->z = 2; // Default "up" is scaled up by two to keep consistant with normal case
        return;
    }

    u32 x = location.x;
    u32 y = location.y;

    assertf(x != 0 && x < depths.cols - 1 && y != 0 && y < depths.rows - 1,
        "Location cannot be on the edge of depths");

    // The partial derivatives dz/dx and dz/dy are approximated by finite differences.
    // dz_dx = (depth(x+1) - depth(x-1)) / 2
    // dz_dy = (depth(y+1) - depth(y-1)) / 2
    // The normal vector is (-dz/dx, -dz/dy, 1).
    // To keep it integer, we can use a scaled normal (-2*dz/dx, -2*dz/dy, 2).
    
    i32 delta_x = (i32) MAT(depths, y, x + 1) - (i32) MAT(depths, y, x - 1);
    i32 delta_y = (i32) MAT(depths, y + 1, x) - (i32) MAT(depths, y - 1, x);

    point_normal->x = -delta_x;
    point_normal->y = -delta_y;
    point_normal->z = 2;
}

/**
 * @brief Computes the integer square root of a 32-bit unsigned integer.
 * @param n The number to find the square root of.
 * @return The floor of the square root of n.
 */
static u32 isqrt32(u32 n) {
    if (n == 0) return 0;
    u32 root = 0;
    // The second-to-top bit of a 32-bit number
    u32 bit = (1UL << 30); 
    while (bit > n) {
        bit >>= 2;
    }
    while (bit != 0) {
        if (n >= root + bit) {
            n -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return root;
}

/**
 * @brief Computes un-normalized principal curvature directions for a u8 depth matrix.
 * Intermediate calculations are done with s32 to guarantee no overflow.
 *
 * @param k1_fp !Will be returned in fixed-point format with CURVATURE_FRACTIONAL_BITS bit additional precision
 * @param k2_fp !Will be returned in fixed-point format with CURVATURE_FRACTIONAL_BITS bit additional precision
 * @param dir1 Output vector for the first principal direction.
 * @param dir2 Output vector for the second principal direction.
 * @param depths Input depth matrix of u8 values.
 * @param location The (x, y) location on the matrix.
 */
void get_principal_curvatures_u8(i32* k1_fp, i32* k2_fp, vec3d* dir1, vec3d* dir2, mat_u8 depths, vec2d location) {
    assertf(is_vec2d_positive(location), "location had negative coords");
    u32 x = location.x;
    u32 y = location.y;

    if (x < 1 || x >= depths.cols - 1 ||
        y < 1 || y >= depths.rows - 1) {
        *k1_fp = 0; *k2_fp = 0;
        dir1->x = 1; dir1->y = 0; dir1->z = 0;
        dir2->x = 0; dir2->y = 1; dir2->z = 0;
        return;
    }

    // --- Hessian elements (using i32 for safety) ---
    i32 z_c = MAT(depths, y, x);
    i32 H_xx = (i32) MAT(depths, y, x + 1) - 2 * z_c + (i32) MAT(depths, y, x - 1);
    i32 H_yy = (i32) MAT(depths, y + 1, x) - 2 * z_c + (i32) MAT(depths, y - 1, x);
    i32 H_xy = ((i32) MAT(depths, y + 1, x + 1) - (i32) MAT(depths, y + 1, x - 1) -
                (i32) MAT(depths, y - 1, x + 1) + (i32) MAT(depths, y - 1, x - 1)) / 4;

    // --- Eigenvector Calculation ---
    i32 trace = H_xx + H_yy;
    i32 diff = H_yy - H_xx;
    i32 two_H_xy = 2 * H_xy;

    vec2d dir1_xy;

    // Check for a flat or perfectly spherical (umbilic) point.
    // In this case, curvature is the same in all directions, so the
    // principal directions are undefined. We assign a stable default.
    if (diff == 0 && two_H_xy == 0) {
        // Curvatures are equal (equal to H_xx and H_yy).
        *k1_fp = H_xx << CURVATURE_FRACTIONAL_BITS;
        *k2_fp = H_xx << CURVATURE_FRACTIONAL_BITS;

        // Assign arbitrary orthogonal vectors for the directions.
        dir1_xy.x = 1;
        dir1_xy.y = 0;
    } else {
        // Standard case: the surface has distinct principal curvatures.
        u32 discriminant_sq = (u32) (diff * diff) + (u32) (two_H_xy * two_H_xy);
        i32 sqrt_disc = (i32) isqrt32(discriminant_sq);

        // Curvatures are the eigenvalues (Tr +/- sqrt(disc))/2
        *k1_fp = ((trace + sqrt_disc) << CURVATURE_FRACTIONAL_BITS) / 2;
        *k2_fp = ((trace - sqrt_disc) << CURVATURE_FRACTIONAL_BITS) / 2;

        // Use the robust method to find a non-zero eigenvector.
        dir1_xy.x = two_H_xy;
        dir1_xy.y = diff + sqrt_disc;

        // If that resulted in a zero vector, use the other valid eigenvector formula.
        if (dir1_xy.x == 0 && dir1_xy.y == 0) {
            dir1_xy.x = diff - sqrt_disc;
            dir1_xy.y = -two_H_xy;
        }
    }
    
    // The other eigenvector is orthogonal
    vec2d dir2_xy = { -dir1_xy.y, dir1_xy.x };

    // --- Lift 2D direction vectors to the 3D tangent plane ---
    // First-order differences fit in i16, but use i32 for consistency
    i32 delta_x = (i32) MAT(depths, y, x + 1) - (i32) MAT(depths, y, x - 1);
    i32 delta_y = (i32) MAT(depths, y + 1, x) - (i32) MAT(depths, y - 1, x);

    dir1->x = dir1_xy.x;
    dir1->y = dir1_xy.y;
    dir1->z = (dir1_xy.x * delta_x + dir1_xy.y * delta_y) / 2;

    dir2->x = dir2_xy.x;
    dir2->y = dir2_xy.y;
    dir2->z = (dir2_xy.x * delta_x + dir2_xy.y * delta_y) / 2;
}

void print_features(features_t f) {
    printf("features: value=%u min_depth=%u max_depth=%u mean_depth=%u principal_curvature_1=%d(%d) principal_curvature_2=%d(%d) pose_fully_defined=%d\n",
        f.value,
        f.min_depth,
        f.max_depth,
        f.mean_depth,
        f.principal_curvature_1_fp >> CURVATURE_FRACTIONAL_BITS, f.principal_curvature_1_fp,
        f.principal_curvature_2_fp >> CURVATURE_FRACTIONAL_BITS, f.principal_curvature_2_fp,
        f.pose_fully_defined
    );
}

void print_pose(pose_t p) {
    printf("pose:\n");
    printf("  point_normal: x=%d y=%d z=%d\n",
        (int)p.point_normal.x, (int)p.point_normal.y, (int)p.point_normal.z);
    printf("  curvature_direction_1: x=%d y=%d z=%d\n",
        (int)p.curvature_direction_1.x, (int)p.curvature_direction_1.y, (int)p.curvature_direction_1.z);
    printf("  curvature_direction_2: x=%d y=%d z=%d\n",
        (int)p.curvature_direction_2.x, (int)p.curvature_direction_2.y, (int)p.curvature_direction_2.z);
    printf("  pose_fully_defined: %d\n", p.pose_fully_defined);
}
