#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <vector>

#include "image.h"
#include "math.h"

void draw_line(Canvas &canvas, int x0, int y0, int x1, int y1, const Color &color);
void visualizeDepth(Canvas &canvas, const std::vector<float> &zbuffer);
void rasterizeTriangleScanline(Canvas &canvas,
                               std::vector<float> &zbuffer,
                               Vec3 v0,
                               Vec3 v1,
                               Vec3 v2,
                               const Color &color);

#endif
