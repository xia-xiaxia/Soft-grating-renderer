#ifndef IMAGE_H
#define IMAGE_H

class Color
{
public:
    unsigned char r, g, b, a;
    Color(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0, unsigned char _a = 255);
};

class Canvas
{
public:
    int width, height;
    Color *pixels;

    Canvas(int w, int h);
    ~Canvas();

    void set_pixel(int x, int y, const Color &color);
    void clear(const Color &color);
};

void save_as_tga(const char *filename, int w, int h, Color *pixels);

#endif
