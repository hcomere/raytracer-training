#ifndef HITTABLE_H
#define HITTABLE_H

#include <Ray.h>
#include <QVector3D>
#include <Interval.h>

class HitRecord {
public:
    QVector3D m_p;
    QVector3D m_normal;
    double m_t;
    bool m_frontFace;

    void setFaceNormal(const Ray& a_ray, const QVector3D& a_outwardNormal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.
        m_frontFace = QVector3D::dotProduct(a_ray.getDirection(), a_outwardNormal) < 0;
        m_normal = m_frontFace ? a_outwardNormal : -a_outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const = 0;
};

#endif
