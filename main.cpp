#include <iostream>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

#pragma pack(push, 1)
struct RGBColor
{
    uint8_t r, g, b;
};
#pragma pack(pop)

int main()
{
    const int imageWidth = 256;
    const int imageHeight = 256;

    int comp = 3;
    RGBColor black = {255, 200, 0};
    RGBColor data[imageWidth * imageHeight * sizeof(RGBColor)];

    for (int y = 0; y < imageHeight; y++)
    {
        for (int x = 0; x < imageWidth; x++)
        {

            data[y * imageWidth + x] = black;
        }
    }

    stbi_write_png("output.png", 100, 100, 3, (const void *)data, (int)sizeof(RGBColor));

    return 0;
}