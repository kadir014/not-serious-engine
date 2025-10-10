/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/model/model.h"


nsModel *nsModel_new(nsMesh *mesh) {
    nsModel *model = NS_NEW(nsModel);
    NS_MEM_CHECK(model);

    model->mesh = mesh;
    model->xform = nsTransform_zero;
    model->xform_mat = nsMatrix4_identity;

    return model;
}

void nsModel_free(nsModel *model) {
    if (!model) return;

    nsMesh_free(model->mesh);

    NS_FREE(model);
}

void nsModel_set_position(nsModel *model, nsVector3 position) {
    model->xform.position = position;
    // TODO: No need to rebuild the whole matrix, just translate it
    model->xform_mat = nsTransform_to_matrix4(model->xform);
}

nsVector3 nsModel_get_position(nsModel *model) {
    return model->xform.position;
}

void nsModel_set_euler_angles(nsModel *model, nsVector3 rotation) {
    model->xform.rotation = rotation;
    model->xform_mat = nsTransform_to_matrix4(model->xform);
}

nsVector3 nsModel_get_euler_angles(nsModel *model) {
    return model->xform.rotation;
}

void nsModel_set_scale(nsModel *model, nsVector3 scale) {
    model->xform.scale = scale;
    model->xform_mat = nsTransform_to_matrix4(model->xform);
}

nsVector3 nsModel_get_scale(nsModel *model) {
    return model->xform.scale;
}

void nsModel_render(nsModel *model) {
    nsMaterial_set_uniform_matrix4(model->mesh->material, "u_model", model->xform_mat);
    nsMesh_render(model->mesh);
}