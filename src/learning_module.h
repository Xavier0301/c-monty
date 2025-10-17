#ifndef LEARNING_MODULE_H
#define LEARNING_MODULE_H

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

typedef struct object_model_cell_ {
    u32 count;
    vec2d average_location;
    pose_t average_pose;
    features_t average_features;
} object_model_cell;

typedef struct object_model_mat_ {
    u32 rows;
    u32 cols;
    object_model_cell* data;
} object_model_mat;

typedef struct grid_lm_ {
    vec2d grid_size;
    u32 scale;
    // short-term/working memory (buffer) for a yet unclassified object OR a yet un-modeled object
    object_model_mat buffer;
    u32 num_buffered_observations;
    // long-term object memory that models all learnt objects for matching
    object_model_mat* learnt_models;
    u32 num_learnt_models;
} grid_lm;

void init_learning_module(grid_lm* lm, vec2d model_size, vec2d world_size);

void learning_module_explore(grid_lm* lm, features_t features, pose_t pose, vec2d location);
void learning_module_match(grid_lm* lm, features_t features, pose_t pose, vec2d location);

#endif
