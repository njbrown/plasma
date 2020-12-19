#include <iostream>
#include <stdint.h>

#include "plasma.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


using namespace std;

#pragma pack(push, 1)
struct RGBColor
{
    uint8_t r, g, b;

    RGBColor()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    RGBColor(color color)
    {
        r = 255 * color.x();
        g = 255 * color.y();
        b = 255 * color.z();
    }
};
#pragma pack(pop)

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0,0,-1.0f), 0.5f, r))
        return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.1, 0.1, 0.1);
}


int main()
{
    const float aspectRatio = 16.0f/9.0f;
    const int imageWidth = 1024;
    const int imageHeight = (int)(imageWidth / aspectRatio);

    // camera props
    const float focalLength = 1.0f;
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;

    auto origin = vec3(0, 0, 0);
    auto horizontal = vec3(viewportWidth, 0, 0);
    auto vertical = vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focalLength);

    int comp = 3;
    // RGBColor black = {255, 200, 200};
    RGBColor data[imageWidth * imageHeight];

    for (int y = 0; y < imageHeight; y++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            auto u = float(x)/(float)(imageWidth-1);
            auto v = float(y)/(float)(imageHeight-1);
            //std::cout<<u<<", "<<v<<std::endl;

            vec3 dir = unit_vector(lowerLeftCorner + u*horizontal + v*vertical - origin);
            //std::cout<<dir<<std::endl;

            ray r(origin, dir);
            vec3 col = ray_color(r);
            // RGBColor resultColor(col);

            data[y * imageWidth + x] = RGBColor(col);
        }
    }

    stbi_write_png("output.png", imageWidth, imageHeight, 3, (const void *)data, imageWidth * (int)sizeof(RGBColor));

    system("xdg-open output.png");

    return 0;
}