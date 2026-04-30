#ifndef SCENE_H
#define SCENE_H

#include <array>
#include <vector>

#include "image.h"
#include "math.h"

struct Vertex
{
    Vec3 position;
    Vec2 uv;
};

struct CubeMesh
{
    std::vector<Vertex> vertices;
    std::vector<std::array<int, 3>> triangles;
    std::array<Color, 6> faceColors;
};

CubeMesh createCubeMesh();

#endif
