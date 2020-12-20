#ifndef CAMERA_H
#define CAMERA_H

#include "plasma.h"

class Camera
{
    public:
    Camera(float aspectRatio = 16.0f/9.0f, float focalLength = 1.0)
    {
        // const float aspectRatio = 16.0f/9.0f;
        // const float focalLength = 1.0f;
        auto viewportHeight = 2.0;
        auto viewportWidth = aspectRatio * viewportHeight;

        origin = vec3(0, 0, 0);
        horizontal = vec3(viewportWidth, 0, 0);
        vertical = vec3(0, -viewportHeight, 0);// flip the y
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focalLength);
    }

    ray getRay(float u, float v)const {
        return ray(origin, unit_vector(lowerLeftCorner + u*horizontal + v*vertical - origin));
    }

    private:
    vec3 origin;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};

#endif