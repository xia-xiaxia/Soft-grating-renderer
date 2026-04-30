#include "math.h"

#include <algorithm>
#include <cmath>

Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}

Vec2 Vec2::operator+(const Vec2 &v) const
{
    return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator-(const Vec2 &v) const
{
    return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator*(float scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vec3 Vec3::operator+(const Vec3 &v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 &v) const
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec4::Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

Vec4 Vec4::operator+(const Vec4 &v) const
{
    return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec4 Vec4::operator-(const Vec4 &v) const
{
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vec4 Vec4::operator*(float scalar) const
{
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 Vec4::operator/(float scalar) const
{
    return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vec3 normalize(const Vec3 &v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0)
    {
        return Vec3(v.x / length, v.y / length, v.z / length);
    }
    return v;
}

Vec4 normalize(const Vec4 &v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    if (length != 0)
    {
        return Vec4(v.x / length, v.y / length, v.z / length, v.w / length);
    }
    return v;
}

Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

Vec4 cross(const Vec4 &a, const Vec4 &b)
{
    return Vec4(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0);
}

float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot(const Vec4 &a, const Vec4 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Mat4::Mat4()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Mat4 Mat4::zero()
{
    Mat4 res;
    for (int i = 0; i < 4; ++i)
    {
        res.m[i][i] = 0.0f;
    }
    return res;
}

Mat4 Mat4::translation(float tx, float ty, float tz)
{
    Mat4 res;
    res.m[0][3] = tx;
    res.m[1][3] = ty;
    res.m[2][3] = tz;
    return res;
}

Mat4 Mat4::scaling(float sx, float sy, float sz)
{
    Mat4 res;
    res.m[0][0] = sx;
    res.m[1][1] = sy;
    res.m[2][2] = sz;
    return res;
}

Mat4 Mat4::rotation_x(float angle)
{
    Mat4 res;
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[1][1] = c;
    res.m[1][2] = -s;
    res.m[2][1] = s;
    res.m[2][2] = c;
    return res;
}

Mat4 Mat4::rotation_y(float angle)
{
    Mat4 res;
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[0][0] = c;
    res.m[0][2] = s;
    res.m[2][0] = -s;
    res.m[2][2] = c;
    return res;
}

Mat4 Mat4::rotation_z(float angle)
{
    Mat4 res;
    float c = std::cos(angle);
    float s = std::sin(angle);
    res.m[0][0] = c;
    res.m[0][1] = -s;
    res.m[1][0] = s;
    res.m[1][1] = c;
    return res;
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Vec4 Mat4::operator*(const Vec4 &v) const
{
    Vec4 result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
    return result;
}

Vec3 Mat4::operator*(const Vec3 &v) const
{
    float x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
    float y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
    float z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];
    float w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];

    if (w != 0.0f)
    {
        return Vec3(x / w, y / w, z / w);
    }
    return Vec3(x, y, z);
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float clampf(float v, float lo, float hi)
{
    return std::max(lo, std::min(hi, v));
}
