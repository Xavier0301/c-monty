#include "learning_module.h"

/**
 * Learning modules create a sensorimotor model of the objects/environment they learn
 * 
 * In practice, our learning module needs to do one thing:
 *      Learn models of objects/environments that can later be used for recognizing/matching them.
 * 
 * In tbp.monty, 3d objets are modeled using graphs, 
 * with nodes representing the features sensed at a pose 
 * and with edges representing displacement
 * 
 * In our case, learning modules don't need to be "useful" but they have one requirement:
 * they need to represent the type of computation that a "useful" learning module would have.
 * Let's try to mimmick what a EvidenceGraphLM does 
 * 
 * In a "learning" (exploration) mode, a graph is created and built upon
 * In a "matching" mode, the current stream of observations is matched against existing models
 * 
 * A later htm-like learning module would need to accomodate a new key demand:
 *      1. Given a movement (hence a change in the location layer), what feature do we expect to see next
 * And learning would happen in the following way:
 *      1. Given a sensory perception (hence a change in the feature layer), what location do we associate with it
 *      2. How wrong were we about our predicted features
 */

 void init_object_model_mat(object_model_mat* object_model, vec2d model_size) {
    object_model->rows = model_size.y;
    object_model->cols = model_size.x;

    object_model->data = calloc(model_size.x * model_size.y, sizeof(*object_model->data));
 }

void init_learning_module(grid_lm* lm, vec2d model_size, vec2d world_size) {
    init_object_model_mat(&lm->buffer, model_size);
    lm->num_buffered_observations = 0;

    lm->num_learnt_models = 0;
    lm->learnt_models = NULL;

    lm->grid_size = model_size;
    lm->scale = world_size.x / model_size.x;

    assert(world_size.x / model_size.x == world_size.y / model_size.y, "model/world size incorrect");
}

u32 incremental_average(u32 last_average, u32 next_element, u32 count) {
    return (count * next_element + last_average) / (count + 1);
}

void learning_module_explore(grid_lm* lm, features_t features, pose_t pose, vec2d world_location) {
    vec2d l = {
        .x = world_location.x / lm->scale,
        .y = world_location.y / lm->scale
    };

    if(MAT(lm->buffer, l.y, l.x).count == 0) {
        MAT(lm->buffer, l.y, l.x).count = 1;
        MAT(lm->buffer, l.y, l.x).average_location = world_location;
        
        MAT(lm->buffer, l.y, l.x).average_pose = pose;
        MAT(lm->buffer, l.y, l.x).average_features = features;
    } else {
        MAT(lm->buffer, l.y, l.x).count += 1;

    }
}

void learning_module_match(grid_lm* lm, features_t features, pose_t pose, vec2d world_location) {
    if(lm->num_learnt_models == 0) return;
}
