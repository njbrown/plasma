#include <iostream>
#include <stdint.h>

#include "plasma.h"
#include "scene.h"
#include "camera.h"

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
    
    Camera camera(aspectRatio, 1.0);
    int samplesPerPixel = 100;
    float colorScale = 1.0/samplesPerPixel;

    int comp = 3;
    // RGBColor black = {255, 200, 200};
    RGBColor *data = new RGBColor[imageWidth * imageHeight];
    color *image = new color[imageWidth * imageHeight];

    for (int y = 0; y < imageHeight; y++)
    {
        std::cerr << "\rScanlines remaining: " << imageHeight - y<< ' ' << std::flush;
        for (int x = 0; x < imageWidth; x++)
        {
            color pixelCol(0,0,0);
            for(int s = 0; s < samplesPerPixel; s++) {
                auto u = float(x + random_float())/(float)(imageWidth-1);
                auto v = float(y + random_float())/(float)(imageHeight-1);

                ray r = camera.getRay(u, v);
                vec3 col = ray_color(r, scene);
                pixelCol += col;
                
            }

            image[y * imageWidth + x] = pixelCol * colorScale;
        }
    }

    // average out samples

    // convert to writeable image
    for(int i = 0;i<imageWidth * imageHeight; i++)
        data[i] = RGBColor(image[i]);

    stbi_write_png("output.png", imageWidth, imageHeight, 3, (const void *)data, imageWidth * (int)sizeof(RGBColor));

    system("xdg-open output.png");

    delete data;
    delete image;

    return 0;
}