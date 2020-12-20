#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "plasma.h"
#include "vec3.h"
#include "ray.h"

struct RayHitResult
{
    vec3 point;
    vec3 normal;
    float t;
    bool isFrontFace;

    inline void setFaceNormal(const ray& r, const vec3& outward_normal) {
        isFrontFace = dot(r.direction(), outward_normal) < 0;
        normal = isFrontFace ? outward_normal :-outward_normal;
    }
};

class Hittable
{
    public:
    virtual bool hit(const ray& r, double t_min, double t_max, RayHitResult& rec) const = 0;
};

using HittablePtr = std::shared_ptr<Hittable>;

class Scene : public Hittable
{
    std::vector<HittablePtr> sceneObjects;

    public:
    void addObject(HittablePtr object)
    {
        sceneObjects.push_back(object);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, RayHitResult& rec) const override;
};
using ScenePtr = std::shared_ptr<Scene>;

class Sphere : public Hittable
{
    public:
    Sphere() {}
    Sphere(point3 pos, float r) : center(pos), radius(r) {};

    virtual bool hit(const ray& r, double t_min, double t_max, RayHitResult& rec) const override;

    public:
    vec3 center;
    float radius;
};

using SpherePtr = std::shared_ptr<Sphere>;


#endif