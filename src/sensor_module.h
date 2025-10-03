#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "math.h"

#include "types.h"
#include "tensor.h"
#include "array.h"
#include "grid_environment.h"
#include "location.h"
#include "interfaces.h"

void sensor_module(features_t* features, pose_t* pose, grid_t patch, vec2d location);

void get_point_normal_u8(vec3d* point_normal, mat_u8 depths, vec2d location);
void get_principal_curvatures_u8(i32* k1_fp, i32* k2_fp, vec3d* dir1, vec3d* dir2, mat_u8 depths, vec2d location);

void print_features(features_t f);
void print_pose(pose_t p);

#endif
