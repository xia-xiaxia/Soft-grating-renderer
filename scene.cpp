#include "scene.h"

CubeMesh createCubeMesh()
{
    CubeMesh cube;

    cube.vertices = {
        // back face
        Vertex{Vec3(-1, -1, -1), Vec2(0, 0)},
        Vertex{Vec3(1, -1, -1), Vec2(1, 0)},
        Vertex{Vec3(1, 1, -1), Vec2(1, 1)},
        Vertex{Vec3(-1, 1, -1), Vec2(0, 1)},

        // front face
        Vertex{Vec3(-1, -1, 1), Vec2(0, 0)},
        Vertex{Vec3(1, -1, 1), Vec2(1, 0)},
        Vertex{Vec3(1, 1, 1), Vec2(1, 1)},
        Vertex{Vec3(-1, 1, 1), Vec2(0, 1)},

        // left face
        Vertex{Vec3(-1, -1, -1), Vec2(0, 0)},
        Vertex{Vec3(-1, -1, 1), Vec2(1, 0)},
        Vertex{Vec3(-1, 1, 1), Vec2(1, 1)},
        Vertex{Vec3(-1, 1, -1), Vec2(0, 1)},

        // right face
        Vertex{Vec3(1, -1, -1), Vec2(0, 0)},
        Vertex{Vec3(1, -1, 1), Vec2(1, 0)},
        Vertex{Vec3(1, 1, 1), Vec2(1, 1)},
        Vertex{Vec3(1, 1, -1), Vec2(0, 1)},

        // bottom face
        Vertex{Vec3(-1, -1, -1), Vec2(0, 0)},
        Vertex{Vec3(1, -1, -1), Vec2(1, 0)},
        Vertex{Vec3(1, -1, 1), Vec2(1, 1)},
        Vertex{Vec3(-1, -1, 1), Vec2(0, 1)},

        // top face
        Vertex{Vec3(-1, 1, -1), Vec2(0, 0)},
        Vertex{Vec3(1, 1, -1), Vec2(1, 0)},
        Vertex{Vec3(1, 1, 1), Vec2(1, 1)},
        Vertex{Vec3(-1, 1, 1), Vec2(0, 1)}
    };

    cube.triangles = {
        {0, 1, 2}, {0, 2, 3},
        {4, 6, 5}, {4, 7, 6},
        {8, 9, 10}, {8, 10, 11},
        {12, 14, 13}, {12, 15, 14},
        {16, 17, 18}, {16, 18, 19},
        {20, 22, 21}, {20, 23, 22}
    };

    cube.faceColors = {
        Color(255, 0, 0),
        Color(0, 255, 0),
        Color(0, 0, 255),
        Color(255, 255, 0),
        Color(255, 0, 255),
        Color(0, 255, 255)
    };

    return cube;
}