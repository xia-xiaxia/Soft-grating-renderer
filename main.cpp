#include <iostream>
#include <string>
#include <vector>

#include "image.h"
#include "rasterizer.h"
#include "scene.h"
#include "transform.h"

struct ScreenVertex
{
    Vec3 position;
    Vec2 uv;
    float invW;
};

int main(int argc, char **argv)
{
    int w = 800;
    int h = 600;
    Canvas canvas(w, h);
    std::vector<float> depth_buffer(w * h, 1e9f);
    canvas.clear(Color(0, 0, 0));

    bool enableWireframe = false;
    bool enableFill = true;
    bool enableDepthVis = false;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--wire" || arg == "--wireframe")
        {
            enableWireframe = true;
        }
        else if (arg == "--fill")
        {
            enableFill = true;
        }
        else if (arg == "--no-fill")
        {
            enableFill = false;
        }
        else if (arg == "--depth")
        {
            enableDepthVis = true;
        }
    }

    CubeMesh cube = createCubeMesh();

    Mat4 model = modelMat(Vec3(1, 1, 1), 0.6f, 0.8f, 0.0f, Vec3(0, 0, 0));
    std::cout << "Model Matrix:" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            std::cout << model.m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    Mat4 view = lookAt(Vec3(0, 0, 5), Vec3(0, 0, 0), Vec3(0, 1, 0));
    Mat4 projection = perspective(3.14159f / 4.0f, static_cast<float>(w) / h, 0.1f, 100.0f);
    std::vector<ScreenVertex> clipVerts;
    clipVerts.reserve(cube.vertices.size());
    for (const auto &v : cube.vertices)
    {
        ScreenVertex sv;
        Vec4 clip = computeFinalMatrix(model, view, projection, v.position);
        if (std::abs(clip.w) < 1e-6f)
        {
            clip.w = (clip.w >= 0.0f) ? 1e-6f : -1e-6f;
        }

        sv.invW = 1.0f / clip.w;
        Vec4 ndc = clip / clip.w;
        sv.position = Vec3(
            (ndc.x * 0.5f + 0.5f) * w,
            (1.0f - (ndc.y * 0.5f + 0.5f)) * h,
            ndc.z * 0.5f + 0.5f);
        sv.uv = v.uv;
        clipVerts.push_back(sv);
    }

    if (enableFill || enableWireframe)
    {
        for (size_t i = 0; i < cube.triangles.size(); ++i)
        {
            const auto &tri = cube.triangles[i];
            Vec3 a = clipVerts[tri[0]].position;
            Vec3 b = clipVerts[tri[1]].position;
            Vec3 c = clipVerts[tri[2]].position;
            Color faceColor = cube.faceColors[i / 2];

            if (enableFill)
            {
                rasterizeTriangleScanline(canvas, depth_buffer, a, b, c, faceColor);
            }
            if (enableWireframe)
            {
                draw_line(canvas, static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), Color(0, 255, 0));
                draw_line(canvas, static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(c.x), static_cast<int>(c.y), Color(0, 255, 0));
                draw_line(canvas, static_cast<int>(c.x), static_cast<int>(c.y), static_cast<int>(a.x), static_cast<int>(a.y), Color(0, 255, 0));
            }
        }
    }

    if (enableDepthVis)
    {
        visualizeDepth(canvas, depth_buffer);
    }

    std::cout << "Rendered cube with " << cube.triangles.size() << " triangles." << std::endl;
    save_as_tga("op_3t_.tga", w, h, canvas.pixels);

    return 0;
}
