/*!
 * @file
 * @brief This file contains implementation of cpu side for phong shading.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <assert.h>
#include <math.h>

#include <student/buffer.h>
#include <student/bunny.h>
#include <student/camera.h>
#include <student/gpu.h>
#include <student/linearAlgebra.h>
#include <student/mouseCamera.h>
#include <student/student_cpu.h>
#include <student/student_pipeline.h>
#include <student/student_shader.h>
#include <student/swapBuffers.h>
#include <student/uniforms.h>
#include <student/vertexPuller.h>

#include <student/globals.h>

/**
 * @brief This structure contains all global variables for this method.
 */
struct PhongVariables {
    /// This variable contains GPU handle.
    GPU gpu;
    /// This variable contains light poistion in world-space.
    Vec3 lightPosition;
    /// vertices
    BufferID vbo;
    /// indices
    BufferID ebo;
    /// Program
    ProgramID program;
    /// puller
    VertexPullerID puller;
} phong;  ///<instance of all global variables for triangle example.


/// \addtogroup cpu_side Úkoly v cpu části
/// @{


void phong_onInit(int32_t width, int32_t height) {
    // create gpu
    phong.gpu = cpu_createGPU();
    // set viewport size
    cpu_setViewportSize(phong.gpu, (size_t)width, (size_t)height);
    // init matrices
    cpu_initMatrices(width, height);
    // init lightPosition
    init_Vec3(&phong.lightPosition, 1000.f, 1000.f, 1000.f);

    // uniformne premenne
    cpu_reserveUniform(phong.gpu, "viewMatrix", UNIFORM_MAT4);
    cpu_reserveUniform(phong.gpu, "projectionMatrix", UNIFORM_MAT4);
    cpu_reserveUniform(phong.gpu, "cameraPosition", UNIFORM_VEC3);
    cpu_reserveUniform(phong.gpu, "lightPosition", UNIFORM_VEC3);

    phong.program = cpu_createProgram(phong.gpu); // program

    // shadere
    cpu_attachVertexShader(phong.gpu, phong.program, phong_vertexShader);
    cpu_attachFragmentShader(phong.gpu, phong.program, phong_fragmentShader);

    cpu_setAttributeInterpolation(phong.gpu, phong.program, 0, ATTRIB_VEC3, SMOOTH);
    cpu_setAttributeInterpolation(phong.gpu, phong.program, 1, ATTRIB_VEC3, SMOOTH);

    // buffery
    cpu_createBuffers(phong.gpu, 1, &phong.vbo);
    cpu_createBuffers(phong.gpu, 1, &phong.ebo);

    cpu_bufferData(phong.gpu, phong.vbo, sizeof(bunnyVertices), bunnyVertices);
    cpu_bufferData(phong.gpu, phong.ebo, sizeof(bunnyIndices), bunnyIndices);

    // pullers
    cpu_createVertexPullers(phong.gpu, 1, &phong.puller);
    cpu_setVertexPullerHead(phong.gpu, phong.puller, 0, phong.vbo, 0, 6*sizeof(float));
    cpu_setVertexPullerHead(phong.gpu, phong.puller, 1, phong.vbo, 3*sizeof(float), 6*sizeof(float));

    cpu_enableVertexPullerHead(phong.gpu, phong.puller, 0);
    cpu_enableVertexPullerHead(phong.gpu, phong.puller, 1);

    cpu_setIndexing(phong.gpu, phong.puller, phong.ebo, 2);
}

/// @}

void phong_onExit() { cpu_destroyGPU(phong.gpu); }

/// \addtogroup cpu_side
/// @{


void phong_onDraw(SDL_Surface* surface) {
    assert(surface != NULL);

    // clear depth buffer
    cpu_clearDepth(phong.gpu, +INFINITY);
    Vec4 color;
    init_Vec4(&color, .1f, .1f, .1f, 1.f);
    // clear color buffer
    cpu_clearColor(phong.gpu, &color);

    cpu_useProgram(phong.gpu, phong.program);
    cpu_bindVertexPuller(phong.gpu, phong.puller);

    UniformLocation locations[4];
    locations[0] = getUniformLocation(phong.gpu, "viewMatrix");
    locations[1] = getUniformLocation(phong.gpu, "projectionMatrix");
    locations[2] = getUniformLocation(phong.gpu, "lightPosition");
    locations[3] = getUniformLocation(phong.gpu, "cameraPosition");

    cpu_uniformMatrix4fv(phong.gpu, locations[0], (float*) &viewMatrix);
    cpu_uniformMatrix4fv(phong.gpu, locations[1], (float*) &projectionMatrix);
    cpu_uniform3f(phong.gpu, locations[2], phong.lightPosition.data[0], phong.lightPosition.data[1], phong.lightPosition.data[2]);
    cpu_uniform3f(phong.gpu, locations[3], cameraPosition.data[0], cameraPosition.data[1], cameraPosition.data[2]);

    cpu_drawTriangles(phong.gpu, 6276);
    // copy image from gpu to SDL surface
    cpu_swapBuffers(surface, phong.gpu);
}

/// @}
