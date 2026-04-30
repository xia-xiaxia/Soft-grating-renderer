#include "transform.h"

#include <cmath>

Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 z = normalize(eye - target);
    Vec3 x = normalize(cross(up, z));
    Vec3 y = normalize(cross(z, x));
    Mat4 res;
    res.m[0][0] = x.x;
    res.m[0][1] = x.y;
    res.m[0][2] = x.z;
    res.m[1][0] = y.x;
    res.m[1][1] = y.y;
    res.m[1][2] = y.z;
    res.m[2][0] = z.x;
    res.m[2][1] = z.y;
    res.m[2][2] = z.z;
    res.m[3][0] = 0;
    res.m[3][1] = 0;
    res.m[3][2] = 0;
    res.m[0][3] = -dot(x, eye);
    res.m[1][3] = -dot(y, eye);
    res.m[2][3] = -dot(z, eye);
    res.m[3][3] = 1;
    return res;
}

Mat4 modelMat(const Vec3 &scale, const float &angleX, const float &angleY, const float &angleZ, const Vec3 &translation)
{
    Mat4 s = Mat4::scaling(scale.x, scale.y, scale.z);
    Mat4 rx = Mat4::rotation_x(angleX);
    Mat4 ry = Mat4::rotation_y(angleY);
    Mat4 rz = Mat4::rotation_z(angleZ);
    Mat4 t = Mat4::translation(translation.x, translation.y, translation.z);
    return t * rz * ry * rx * s;
}

Mat4 perspective(float fov, float aspect, float near, float far)
{
    Mat4 res = Mat4::zero();
    float tanHalfFov = std::tan(fov / 2.0f);
    res.m[0][0] = 1.0f / (aspect * tanHalfFov);
    res.m[1][1] = 1.0f / tanHalfFov;
    res.m[2][2] = -(far + near) / (far - near);
    res.m[2][3] = -(2.0f * far * near) / (far - near);
    res.m[3][2] = -1.0f;
    return res;
}

Mat4 viewport(int x, int y, int w, int h)
{
    Mat4 res = Mat4::zero();
    res.m[0][0] = w / 2.0f;
    res.m[0][3] = x + w / 2.0f;
    res.m[1][1] = -h / 2.0f;
    res.m[1][3] = y + h / 2.0f;
    res.m[2][2] = 0.5f;
    res.m[2][3] = 0.5f;
    res.m[3][3] = 1.0f;
    return res;
}

Vec4 computeFinalMatrix(const Mat4 &model,
                        const Mat4 &view,
                        const Mat4 &projection,
                        const Vec3 &vertex)
{
    Vec4 hv(vertex.x, vertex.y, vertex.z, 1.0f);
    return projection * view * model * hv;
}
