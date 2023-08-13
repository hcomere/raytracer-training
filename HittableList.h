#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <Hittable.h>
#include <Ray.h>

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> m_objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> a_object) { add(a_object); }

    void clear() { m_objects.clear(); }

    void add(std::shared_ptr<Hittable> a_object) {
        m_objects.push_back(a_object);
    }

    bool hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const override {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = a_tInterval.m_max;

        for (const auto& object : m_objects) {
            if (object->hit(a_ray, Interval(a_tInterval.m_min, closestSoFar), tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.m_t;
                a_outHit = tempRec;
            }
        }

        return hitAnything;
    }
};

#endif
