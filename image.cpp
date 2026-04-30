#include "image.h"

#include <fstream>

Color::Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
    : r(_r), g(_g), b(_b), a(_a) {}

Canvas::Canvas(int w, int h) : width(w), height(h)
{
    pixels = new Color[width * height];
}

Canvas::~Canvas()
{
    delete[] pixels;
}

void Canvas::set_pixel(int x, int y, const Color &color)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        pixels[y * width + x] = color;
    }
}

void Canvas::clear(const Color &color)
{
    for (int i = 0; i < width * height; ++i)
    {
        pixels[i] = color;
    }
}

void save_as_tga(const char *filename, int w, int h, Color *pixels)
{
    unsigned char header[18] = {0};
    header[2] = 2;
    header[12] = w & 0xFF;
    header[13] = (w >> 8) & 0xFF;
    header[14] = h & 0xFF;
    header[15] = (h >> 8) & 0xFF;
    header[16] = 32;

    std::ofstream out(filename, std::ios::binary);
    out.write((char *)header, 18);

    for (int i = 0; i < w * h; i++)
    {
        out.put(pixels[i].b);
        out.put(pixels[i].g);
        out.put(pixels[i].r);
        out.put(pixels[i].a);
    }
    out.close();
}
