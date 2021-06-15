#include "Graphics.h"

#include <libc/string.h>

namespace graphics {

static Framebuffer _framebuffer;

void initialize(const Framebuffer* fb) {
    memcpy(&_framebuffer, fb, sizeof(Framebuffer));

    plot(5, 5, 0xFFFFFF);
    fillRect(0, 0, _framebuffer.width, _framebuffer.height, 0xFFFFFF);
}

void plot(size_t x, size_t y, uint32_t color)
{
    if ((x <= _framebuffer.width) &&
        (y <= _framebuffer.height))
    {
        uint8_t *pixel = (reinterpret_cast<uint8_t *>(_framebuffer.address)) + (y * (_framebuffer.pitch)) + (x * _framebuffer.bpp);

        if (_framebuffer.bpp == 4)
        {
            pixel[0] = (color >> 0) & 0xff;
            pixel[1] = (color >> 8) & 0xff;
            pixel[2] = (color >> 16) & 0xff;
            pixel[4] = 0xff;
        }
        else if (_framebuffer.bpp == 3)
        {
            pixel[0] = color & 0xff;
            pixel[1] = (color >> 8) & 0xff;
            pixel[2] = (color >> 16) & 0xff;
        }
    }
}

void fillRect(size_t x, size_t y, size_t width, size_t height, uint32_t color)
{
    for(size_t i = x; i <= width; i++) {
        for(size_t j = y; j <= height; j++) {
            plot(i, j, color);
        }
    }
}

}