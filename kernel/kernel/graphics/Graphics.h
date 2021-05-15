#pragma once

#include <libruntime/Types.h>

namespace hegel::graphics {

struct Framebuffer
{
    uintptr_t address;
    size_t width;
    size_t height;
    size_t pitch; // bytes per row
    size_t bpp; // bytes per pixel
};

void initialize(const Framebuffer* fb);
void plot(size_t x, size_t y, uint32_t color);
void fillRect(size_t x, size_t y, size_t width, size_t height, uint32_t color);

}