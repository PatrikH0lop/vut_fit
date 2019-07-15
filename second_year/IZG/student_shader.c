/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
    Uniforms uh = gpu_getUniformsHandle(gpu);
    // used matrices
    UniformLocation locations[2];
    struct Mat4 *matrices[2];
    struct Vec3 *ova[4];
    locations[0] = getUniformLocation(gpu, "viewMatrix");
    locations[1] = getUniformLocation(gpu, "projectionMatrix");
    matrices[0] = (struct Mat4 *) shader_interpretUniformAsMat4(uh, locations[0]);
    matrices[1] = (struct Mat4 *) shader_interpretUniformAsMat4(uh, locations[1]);
    Vec3 *position = (struct Vec3 *) vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
    Mat4 matrix_to_p;
    Vec4 pos_to_p;
    multiply_Mat4_Mat4(&matrix_to_p, matrices[1], matrices[0]);
    copy_Vec3Float_To_Vec4(&pos_to_p, position, 1.0f);
    multiply_Mat4_Vec4(&output->gl_Position, &matrix_to_p, &pos_to_p);
    ova[0] = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
    ova[1] = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);
    ova[2] = (struct Vec3*) vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
    ova[3] = (struct Vec3*) vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);
    init_Vec3(ova[0], ova[2]->data[0], ova[2]->data[1], ova[2]->data[2]);
    init_Vec3(ova[1], ova[3]->data[0], ova[3]->data[1], ova[3]->data[2]);
}


// help function
float max(float a, float b) {
    return (a > b) ? a : b;
}


void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
    Uniforms uh = gpu_getUniformsHandle(gpu);
    Vec3 *l, *c, *pp, *np;
    pp = (struct Vec3 *) fs_interpretInputAttributeAsVec3(gpu, input, 0);
    np = (struct Vec3 *) fs_interpretInputAttributeAsVec3(gpu, input, 1);
    l =  (struct Vec3 *) shader_interpretUniformAsVec3(uh, getUniformLocation(gpu, "lightPosition"));
    c =  (struct Vec3 *) shader_interpretUniformAsVec3(uh, getUniformLocation(gpu, "cameraPosition"));

    Vec3 vn, vc, vl, vr;
    normalize_Vec3(&vn, np);
    normalize_Vec3(&vc, c);

    float l_x = l->data[0] - pp->data[0];
    float l_y = l->data[1] - pp->data[1];
    float l_z = l->data[2] - pp->data[2];
    init_Vec3(&vl, l_x, l_y, l_z);
    normalize_Vec3(&vl, &vl);
    init_Vec3(&vr, -vl.data[0], -vl.data[1], -vl.data[2]);
    reflect(&vr, &vr, &vn);

    //////////////////////////////// DIFUZNA FARBA /////////////////////////////
    float init_diff = dot_Vec3(&vn, &vl);
    init_diff = max(init_diff, 0);

    struct Vec3 green; // zelena
    init_Vec3(&green, 0, 1, 0);
    struct Vec3 white; // biela
    init_Vec3(&white, 1, 1, 1);

    struct Vec3 interpolated_color;

    if (vn.data[1] < 0) { // ak je normala zaporna, farba je zelena
        init_Vec3(&interpolated_color, green.data[0], green.data[1], green.data[2]);
    } else { // inak je interpolovana
        float t = vn.data[1] * vn.data[1]; // int. parameter
        float int_x = (1 - t) * green.data[0] + t * white.data[0];
        float int_y = (1 - t) * green.data[1] + t * white.data[1];
        float int_z = (1 - t) * green.data[2] + t * white.data[2];
        init_Vec3(&interpolated_color, int_x, int_y, int_z);
    }

    struct Vec3 diffuse_color; // vysledna difuzna farba
    multiply_Vec3_Float(&diffuse_color, &interpolated_color, init_diff);

    ////////////////////////////// SPECT FARBA ///////////////////////////////

    struct Vec3 spectacular_color; // vysledna spect. farba
    init_Vec3(&spectacular_color, white.data[0], white.data[1], white.data[2]);

    float spec = dot_Vec3(&vr, &vc); // spect. zlozka
    spec = max(spec, 0);
    spec = (float) pow(spec, 40.0f);

    multiply_Vec3_Float(&spectacular_color, &spectacular_color, spec);

    //////////////////////////////// VYSLEDNA /////////////////////////////////
    float final_x =  spectacular_color.data[0] + diffuse_color.data[0];
    float final_y =  spectacular_color.data[1] + diffuse_color.data[1];
    float final_z =  spectacular_color.data[2] + diffuse_color.data[2];

    Vec3 colorAttribute;
    init_Vec3(&colorAttribute, final_x, final_y, final_z);
    copy_Vec3Float_To_Vec4(&output->color, &colorAttribute, 1.f);
}

/// @}
