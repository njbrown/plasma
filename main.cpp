#include <iostream>
#include <stdint.h>

#include "plasma.h"
#include "scene.h"

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

float hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const HittablePtr& world) {
    RayHitResult rec;
    if (world->hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.1, 0.1, 0.1);
}


int main()
{
    ScenePtr scene(new Scene());
    scene->addObject(SpherePtr(new Sphere(vec3(0,0,-1), 0.5f)));
    scene->addObject(SpherePtr(new Sphere(vec3(0,1,-1), 0.5f)));
    scene->addObject(SpherePtr(new Sphere(vec3(1,0,-1), 0.5f)));
    scene->addObject(SpherePtr(new Sphere(vec3(0,-100.5f,0), 100)));

    const float aspectRatio = 16.0f/9.0f;
    const int imageWidth = 1024;
    const int imageHeight = (int)(imageWidth / aspectRatio);

    // camera props
    const float focalLength = 1.0f;
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;

    auto origin = vec3(0, 0, 0);
    auto horizontal = vec3(viewportWidth, 0, 0);
    auto vertical = vec3(0, -viewportHeight, 0);// flip the y
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
            vec3 col = ray_color(r, scene);
            // RGBColor resultColor(col);

            data[y * imageWidth + x] = RGBColor(col);
        }
    }

    stbi_write_png("output.png", imageWidth, imageHeight, 3, (const void *)data, imageWidth * (int)sizeof(RGBColor));

    system("xdg-open output.png");

    return 0;
}