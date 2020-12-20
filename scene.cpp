#include "scene.h"


bool Scene::hit(const ray& r, double t_min, double t_max, RayHitResult& rec) const
{
    RayHitResult temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto& object : sceneObjects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool Sphere::hit(const ray& r, double t_min, double t_max, RayHitResult& rec) const
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    // rec.normal = (rec.point - center) / radius;
    vec3 outward_normal = (rec.point - center) / radius;
    rec.setFaceNormal(r, outward_normal);
    return true;
}