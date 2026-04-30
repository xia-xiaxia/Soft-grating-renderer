#ifndef MATH_H
#define MATH_H

class Vec2
{
public:
    float x, y;
    Vec2(float _x = 0, float _y = 0);
    Vec2 operator+(const Vec2 &v) const;
    Vec2 operator-(const Vec2 &v) const;
    Vec2 operator*(float scalar) const;
};

class Vec3
{
public:
    float x, y, z;
    Vec3(float _x = 0, float _y = 0, float _z = 0);
    Vec3 operator+(const Vec3 &v) const;
    Vec3 operator-(const Vec3 &v) const;
    Vec3 operator*(float scalar) const;
};

class Vec4
{
public:
    float x, y, z, w;
    Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);
    Vec4 operator+(const Vec4 &v) const;
    Vec4 operator-(const Vec4 &v) const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;
};

class Mat4
{
public:
    float m[4][4];

    Mat4();

    static Mat4 zero();
    static Mat4 translation(float tx, float ty, float tz);
    static Mat4 scaling(float sx, float sy, float sz);
    static Mat4 rotation_x(float angle);
    static Mat4 rotation_y(float angle);
    static Mat4 rotation_z(float angle);

    Mat4 operator*(const Mat4 &other) const;
    Vec4 operator*(const Vec4 &v) const;
    Vec3 operator*(const Vec3 &v) const;
};

Vec3 normalize(const Vec3 &v);
Vec4 normalize(const Vec4 &v);
Vec3 cross(const Vec3 &a, const Vec3 &b);
Vec4 cross(const Vec4 &a, const Vec4 &b);
float dot(const Vec3 &a, const Vec3 &b);
float dot(const Vec4 &a, const Vec4 &b);
float lerp(float a, float b, float t);
float clampf(float v, float lo, float hi);

#endif
