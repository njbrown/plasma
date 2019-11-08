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
    int comp = 3;
    RGBColor black = {255, 0, 0};
    RGBColor data[100 * 100 * sizeof(RGBColor)];

    for (int y = 0; y < 100; y++)
    {
        for (int x = 0; x < 100; x++)
        {

            data[y * 100 + x] = black;
        }
    }

    stbi_write_png("renders/output.png", 100, 100, 3, (const void *)data, (int)sizeof(RGBColor));

    return 0;
}