#ifndef HITTABLE_H
#define HITTABLE_H

#include <Ray.h>
#include <Interval.h>
#include <HitRecord.h>

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const = 0;
};

#endif
