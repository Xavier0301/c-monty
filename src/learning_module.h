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

typedef struct grid_lm_ {
    // short-term/working memory (buffer) for a yet unclassified object OR a yet un-modeled object

    // long-term object memory that models all learnt objects for matching

} grid_lm;

void init_learning_module(grid_lm* lm);

void learning_module_explore(grid_lm* lm, features_t features, pose_t pose);
void learning_module_match(grid_lm* lm, features_t features, pose_t pose);

#endif
