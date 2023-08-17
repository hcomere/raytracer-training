#ifndef WORLD_H
#define WORLD_H

#include <Common.h>
#include <Hittable.h>
#include <Ray.h>

class World : public Hittable
{
    private:
        HittablePtrVector m_objects;

    public:
        World();
        World(HittablePtr a_object);

    public:
        void clear();
        void add(HittablePtr a_object);

    public:
        bool hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const override;
};

#endif
