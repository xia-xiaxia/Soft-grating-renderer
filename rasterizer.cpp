#include "rasterizer.h"

#include <algorithm>
#include <cmath>

void draw_line(Canvas &canvas, int x0, int y0, int x1, int y1, const Color &color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        canvas.set_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        int err2 = err * 2;
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

void visualizeDepth(Canvas &canvas, const std::vector<float> &zbuffer)
{
    float minZ = 1e9f;
    float maxZ = -1e9f;
    for (float z : zbuffer)
    {
        if (z < 1e8f)
        {
            minZ = std::min(minZ, z);
            maxZ = std::max(maxZ, z);
        }
    }

    if (minZ > maxZ)
    {
        return;
    }

    float range = std::max(1e-6f, maxZ - minZ);
    for (int y = 0; y < canvas.height; ++y)
    {
        for (int x = 0; x < canvas.width; ++x)
        {
            int idx = y * canvas.width + x;
            float z = zbuffer[idx];
            if (z >= 1e8f)
            {
                continue;
            }

            float t = clampf((z - minZ) / range, 0.0f, 1.0f);
            unsigned char c = static_cast<unsigned char>(lerp(255.0f, 0.0f, t));
            canvas.set_pixel(x, y, Color(c, c, c));
        }
    }
}

void rasterizeTriangleScanline(Canvas &canvas,
                               std::vector<float> &zbuffer,
                               Vec3 v0,
                               Vec3 v1,
                               Vec3 v2,
                               const Color &color)
{
    if (v1.y < v0.y)
    {
        std::swap(v0, v1);
    }
    if (v2.y < v0.y)
    {
        std::swap(v0, v2);
    }
    if (v2.y < v1.y)
    {
        std::swap(v1, v2);
    }

    float y0 = v0.y;
    float y1 = v1.y;
    float y2 = v2.y;
    if (std::abs(y2 - y0) < 1e-6f)
    {
        return;
    }

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

        if (fy < y1)
        {
            edge(v0, v1, fy, xl, zl);
            edge(v0, v2, fy, xr, zr);
        }
        else
        {
            edge(v1, v2, fy, xl, zl);
            edge(v0, v2, fy, xr, zr);
        }

        if (xl > xr)
        {
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
            if (z < zbuffer[idx])
            {
                zbuffer[idx] = z;
                canvas.set_pixel(x, y, color);
            }
        }
    }
}
