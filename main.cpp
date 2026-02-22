#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <array>
#include <string>

// 1、 数学基础
class Vec2
{
public:
    float x, y;
    Vec2(float _x = 0, float _y = 0) : x(_x), y(_y) {}
    Vec2 operator+(const Vec2 &v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2 &v) const
    {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }
};

class Vec3
{
public:
    float x, y, z;
    Vec3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
    Vec3 operator+(const Vec3 &v) const
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator-(const Vec3 &v) const
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator*(float scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
};

class Vec4
{
public:
    float x, y, z, w;
    Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z), w(_w) {}
    Vec4 operator+(const Vec4 &v) const
    {
        return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    Vec4 operator-(const Vec4 &v) const
    {
        return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    Vec4 operator*(float scalar) const
    {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
};

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
float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vec4 cross(const Vec4 &a, const Vec4 &b)
{
    return Vec4(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0 // 或1
    );
}
float dot(const Vec4 &a, const Vec4 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// 4 x 4 矩阵
class Mat4
{
public:
    float m[4][4];
    Mat4()
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    // 静态方法：快速获取全零矩阵
    static Mat4 zero()
    {
        Mat4 res;
        for (int i = 0; i < 4; ++i)
            res.m[i][i] = 0.0f;
        return res;
    }
    Mat4 operator*(const Mat4 &other) const
    {
        Mat4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    Vec4 operator*(const Vec4 &v) const
    {
        Vec4 result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
        result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
        return result;
    }
    // 处理Vec3的乘法，假设w=1
    Vec3 operator*(const Vec3 &v) const
    {
        // 1. 隐式升级到 Vec4 (w = 1)
        float x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
        float y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
        float z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];
        float w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];

        // 2. 透视除法：回到 3D 空间
        if (w != 0.0f)
        {
            return {x / w, y / w, z / w};
        }
        return {x, y, z};
    }
    // 位移矩阵
    static Mat4 translation(float tx, float ty, float tz)
    {
        Mat4 res;
        res.m[0][3] = tx;
        res.m[1][3] = ty;
        res.m[2][3] = tz;
        return res;
    }
    // 缩放矩阵
    static Mat4 scaling(float sx, float sy, float sz)
    {
        Mat4 res;
        res.m[0][0] = sx;
        res.m[1][1] = sy;
        res.m[2][2] = sz;
        return res;
    }
    // 绕 X 轴旋转矩阵
    static Mat4 rotation_x(float angle)
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
    // 绕 Y 轴旋转矩阵
    static Mat4 rotation_y(float angle)
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
    // 绕 Z 轴旋转矩阵
    static Mat4 rotation_z(float angle)
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
};

// 画布与输出
class Color
{
public:
    unsigned char r, g, b, a;
    Color(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0, unsigned char _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
};
class Canvas
{
public:
    int width, height;
    Color *pixels;
    Canvas(int w, int h) : width(w), height(h)
    {
        pixels = new Color[width * height];
    }
    ~Canvas()
    {
        delete[] pixels;
    }
    void set_pixel(int x, int y, const Color &color)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            pixels[y * width + x] = color;
        }
    }
    void clear(const Color &color)
    {
        for (int i = 0; i < width * height; ++i)
        {
            pixels[i] = color;
        }
    }
};

// 保存为 TGA 文件
void save_as_tga(const char *filename, int w, int h, Color *pixels)
{
    // TGA 文件头 (18 字节)
    unsigned char header[18] = {0};
    header[2] = 2;                // 未压缩的真彩色图像
    header[12] = w & 0xFF;        // 宽度低位
    header[13] = (w >> 8) & 0xFF; // 宽度高位
    header[14] = h & 0xFF;        // 高度低位
    header[15] = (h >> 8) & 0xFF; // 高度高位
    header[16] = 32;              // 32 位深 (RGBA)

    std::ofstream out(filename, std::ios::binary);
    out.write((char *)header, 18);

    // TGA 默认存储顺序通常是 BGRA
    for (int i = 0; i < w * h; i++)
    {
        out.put(pixels[i].b);
        out.put(pixels[i].g);
        out.put(pixels[i].r);
        out.put(pixels[i].a);
    }
    out.close();
}
// Bresenham 直线算法
void draw_line(Canvas &canvas, int x0, int y0, int x1, int y1, const Color &color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy, err2;

    while (true)
    {
        canvas.set_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        err2 = err * 2;
        if (err2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// LookAt 矩阵
Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 z = normalize(eye - target);
    Vec3 x = normalize(cross(up, z));
    Vec3 y = normalize(cross(z, x));
    Mat4 res;
    // 平移部分
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
    // 旋转部分
    res.m[0][3] = -dot(x, eye);
    res.m[1][3] = -dot(y, eye);
    res.m[2][3] = -dot(z, eye);
    res.m[3][3] = 1;
    return res;
}

// 这里 (1, 1, 1) 不是 Vec3(1, 1, 1)！在C++中需要显式调用构造函数。
Mat4 modelMat(const Vec3 &scale = Vec3(1, 1, 1), const float &angleX = 0, const float &angleY = 0, const float &angleZ = 0, const Vec3 &translation = Vec3(0, 0, 0))
{
    Mat4 S = Mat4::scaling(scale.x, scale.y, scale.z);
    Mat4 Rx = Mat4::rotation_x(angleX);
    Mat4 Ry = Mat4::rotation_y(angleY);
    Mat4 Rz = Mat4::rotation_z(angleZ);
    Mat4 T = Mat4::translation(translation.x, translation.y, translation.z);
    return T * Rz * Ry * Rx * S;
}

// (Perspective Projection Matrix) 透视投影矩阵
Mat4 perspective(float fov, float aspect, float near, float far)
{
    Mat4 res = Mat4::zero();
    float tanHalfFov = std::tan(fov / 2.0f);
    res.m[0][0] = 1.0f / (aspect * tanHalfFov);
    res.m[1][1] = 1.0f / (tanHalfFov);
    res.m[2][2] = -(far + near) / (far - near);
    res.m[2][3] = -(2.0f * far * near) / (far - near);
    res.m[3][2] = -1.0f;
    return res;
}

// Viewport 矩阵
Mat4 viewport(int x, int y, int w, int h)
{
    Mat4 res = Mat4::zero();
    res.m[0][0] = w / 2.0f;
    res.m[0][3] = x + w / 2.0f;
    res.m[1][1] = -h / 2.0f;  // 翻转Y轴
    res.m[1][3] = y + h / 2.0f;
    res.m[2][2] = 0.5f;
    res.m[2][3] = 0.5f;
    res.m[3][3] = 1.0f;
    return res;
}

// final Vec3 = Viewport * Projection * View * Model * Vertex
Vec3 computeFinalMatrix(const Mat4 &model, const Mat4 &view, const Mat4 &projection, const Mat4 &viewport, const Vec3 &vertex)
{
    return viewport * projection * view * model * vertex;
}

// 三角形
class Triangle {
    Vec3 v0, v1, v2;
public:
    float min_x, max_x, min_y, max_y;
    Triangle(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2) : v0(_v0), v1(_v1), v2(_v2) {}
    float area() const {
        Vec3 ab = v1 - v0;
        Vec3 ac = v2 - v0;
        Vec3 crossProd = cross(ab, ac);
        return 0.5f * std::sqrt(dot(crossProd, crossProd));
    }
    // 在屏幕上进行光栅化应该只用X、Y坐标，Z坐标用于深度测试。
    bool containsPoint(Vec3& p) const {
        // 只用X、Y坐标计算二维面积
        auto area2D = [](const Vec3& a, const Vec3& b, const Vec3& c) {
            return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) / 2.0f;
        };
        
        float A = area2D(v0, v1, v2);
        if (A < 1e-6) return false;
        
        float A1 = area2D(p, v1, v2);
        float A2 = area2D(v0, p, v2);
        float A3 = area2D(v0, v1, p);

        float a = A/A1;
        float b = A/A2;
        float c = A/A3;
        
        p.z = a*v0.z + b*v1.z + c*v2.z;        
        return std::abs(A - (A1 + A2 + A3)) < 1e-3;
    }

    void getBoundingBox() {
        min_x = std::min({v0.x, v1.x, v2.x});
        max_x = std::max({v0.x, v1.x, v2.x});
        min_y = std::min({v0.y, v1.y, v2.y});
        max_y = std::max({v0.y, v1.y, v2.y});
    }
};

struct tri_color {

};

static inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

static inline float clampf(float v, float lo, float hi) {
    return std::max(lo, std::min(hi, v));
}

void visualizeDepth(Canvas &canvas, const std::vector<float> &zbuffer)
{
    float minZ = 1e9f;
    float maxZ = -1e9f;
    for (float z : zbuffer) {
        if (z < 1e8f) {
            minZ = std::min(minZ, z);
            maxZ = std::max(maxZ, z);
        }
    }
    if (minZ > maxZ) return;
    float range = std::max(1e-6f, maxZ - minZ);
    for (int y = 0; y < canvas.height; ++y) {
        for (int x = 0; x < canvas.width; ++x) {
            int idx = y * canvas.width + x;
            float z = zbuffer[idx];
            if (z >= 1e8f) continue;
            float t = clampf((z - minZ) / range, 0.0f, 1.0f);
            unsigned char c = static_cast<unsigned char>(lerp(255.0f, 0.0f, t));
            canvas.set_pixel(x, y, Color(c, c, c));
        }
    }
}

// 扫描线三角形填充 + Z-buffer
void rasterizeTriangleScanline(Canvas &canvas, std::vector<float> &zbuffer,
                               Vec3 v0, Vec3 v1, Vec3 v2, const Color &color)
{
    // 按 y 排序
    if (v1.y < v0.y) std::swap(v0, v1);
    if (v2.y < v0.y) std::swap(v0, v2);
    if (v2.y < v1.y) std::swap(v1, v2);

    float y0 = v0.y, y1 = v1.y, y2 = v2.y;
    if (std::abs(y2 - y0) < 1e-6f) return; // 退化

    int minY = static_cast<int>(std::ceil(y0));
    int maxY = static_cast<int>(std::floor(y2));
    minY = std::max(minY, 0);
    maxY = std::min(maxY, canvas.height - 1);

    auto edge = [](const Vec3 &a, const Vec3 &b, float y, float &x, float &z) {
        float dy = b.y - a.y;
        float t = (std::abs(dy) < 1e-6f) ? 0.0f : (y - a.y) / dy;
        t = clampf(t, 0.0f, 1.0f);
        x = lerp(a.x, b.x, t);
        z = lerp(a.z, b.z, t);
    };

    for (int y = minY; y <= maxY; ++y)
    {
        float fy = y + 0.5f;
        float xl, zl, xr, zr;

        if (fy < y1) {
            // 上半部分：v0-v1 与 v0-v2
            edge(v0, v1, fy, xl, zl);
            edge(v0, v2, fy, xr, zr);
        } else {
            // 下半部分：v1-v2 与 v0-v2
            edge(v1, v2, fy, xl, zl);
            edge(v0, v2, fy, xr, zr);
        }

        if (xl > xr) {
            std::swap(xl, xr);
            std::swap(zl, zr);
        }

        int minX = static_cast<int>(std::ceil(xl));
        int maxX = static_cast<int>(std::floor(xr));
        minX = std::max(minX, 0);
        maxX = std::min(maxX, canvas.width - 1);

        float span = xr - xl;
        for (int x = minX; x <= maxX; ++x)
        {
            float fx = x + 0.5f;
            float t = (std::abs(span) < 1e-6f) ? 0.0f : (fx - xl) / span;
            t = clampf(t, 0.0f, 1.0f);
            float z = lerp(zl, zr, t);

            int idx = y * canvas.width + x;
            if (z < zbuffer[idx]) {
                zbuffer[idx] = z;
                canvas.set_pixel(x, y, color);
            }
        }
    }
}

int main(int argc, char **argv)
{
    int w = 800, h = 600;
    Canvas canvas(w, h);
    std::vector<float> depth_buffer(w * h, 1e9f);
    canvas.clear(Color(0, 0, 0)); // 背景涂黑

    // 渲染模式开关
    bool enableWireframe = false;
    bool enableFill = true;
    bool enableDepthVis = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--wire" || arg == "--wireframe") {
            enableWireframe = true;
        } else if (arg == "--fill") {
            enableFill = true;
        } else if (arg == "--no-fill") {
            enableFill = false;
        } else if (arg == "--depth") {
            enableDepthVis = true;
        }
    }

    // 立方体顶点（模型空间）
    std::vector<Vec3> cubeVerts = {
        Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(1,  1, -1), Vec3(-1,  1, -1),
        Vec3(-1, -1,  1), Vec3(1, -1,  1), Vec3(1,  1,  1), Vec3(-1,  1,  1)
    };
    // 12 个三角形（每个面 2 个三角形）
    std::vector<std::array<int, 3>> cubeTris = {
        {0, 1, 2}, {0, 2, 3}, // back
        {4, 6, 5}, {4, 7, 6}, // front
        {0, 4, 5}, {0, 5, 1}, // bottom
        {3, 2, 6}, {3, 6, 7}, // top
        {1, 5, 6}, {1, 6, 2}, // right
        {0, 3, 7}, {0, 7, 4}  // left
    };

    viewport(0, 0, w, h);
    Mat4 m = modelMat(Vec3(1, 1, 1), 0.6f, 0.8f, 0.0f, Vec3(0, 0, 0));
    std::cout << "Model Matrix:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m.m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    Mat4 view = lookAt(Vec3(0, 0, 5), Vec3(0, 0, 0), Vec3(0, 1, 0));
    Mat4 projection = perspective(3.14159f / 4.0f, static_cast<float>(w) / h, 0.1f, 100.0f);
    Mat4 vp = viewport(0, 0, w, h);
    std::vector<Vec3> clipVerts;
    clipVerts.reserve(cubeVerts.size());
    for (const auto &v : cubeVerts) {
        clipVerts.push_back(computeFinalMatrix(m, view, projection, vp, v));
    }
    // 扫描线 + Z-buffer
    if (enableFill || enableWireframe) {
        std::array<Color, 6> faceColors = {
            Color(255, 0, 0),   // back
            Color(0, 255, 0),   // front
            Color(0, 0, 255),   // bottom
            Color(255, 255, 0), // top
            Color(255, 0, 255), // right
            Color(0, 255, 255)  // left
        };
        for (size_t i = 0; i < cubeTris.size(); ++i) {
            const auto &tri = cubeTris[i];
            Vec3 a = clipVerts[tri[0]];
            Vec3 b = clipVerts[tri[1]];
            Vec3 c = clipVerts[tri[2]];
            Color faceColor = faceColors[i / 2];
            if (enableFill) {
                rasterizeTriangleScanline(canvas, depth_buffer, a, b, c, faceColor);
            }
            if (enableWireframe) {
                draw_line(canvas, static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), Color(0, 255, 0));
                draw_line(canvas, static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(c.x), static_cast<int>(c.y), Color(0, 255, 0));
                draw_line(canvas, static_cast<int>(c.x), static_cast<int>(c.y), static_cast<int>(a.x), static_cast<int>(a.y), Color(0, 255, 0));
            }
        }
    }
    if (enableDepthVis) {
        visualizeDepth(canvas, depth_buffer);
    }
    std::cout << "Rendered cube with " << cubeTris.size() << " triangles." << std::endl;

    // 保存结果
    save_as_tga("op_3t_.tga", w, h, canvas.pixels);
    

    return 0;
}