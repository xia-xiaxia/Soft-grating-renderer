#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math.h"

Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up);
Mat4 modelMat(const Vec3 &scale = Vec3(1, 1, 1),
              const float &angleX = 0,
              const float &angleY = 0,
              const float &angleZ = 0,
              const Vec3 &translation = Vec3(0, 0, 0));
Mat4 perspective(float fov, float aspect, float near, float far);
Mat4 viewport(int x, int y, int w, int h);
Vec4 computeFinalMatrix(const Mat4 &model,
                        const Mat4 &view,
                        const Mat4 &projection,
                        const Vec3 &vertex);

#endif
