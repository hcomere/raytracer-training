#ifndef SPHERE_H
#define SPHERE_H

#include <memory>

#include <Hittable.h>
#include <Ray.h>
#include <QVector3D>

class Material;

class Sphere : public Hittable {
public:
    Sphere(const QVector3D& a_center, double a_radius, std::shared_ptr<Material> a_material)
        : m_center(a_center)
        , m_radius(a_radius)
        , m_material(a_material)
    {
    }

    bool hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const override
    {
        QVector3D oc = a_ray.getOrigin() - m_center;
        double a = a_ray.getDirection().lengthSquared();
        double half_b = QVector3D::dotProduct(oc, a_ray.getDirection());
        double c = oc.lengthSquared() - m_radius*m_radius;

        double discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (-half_b - sqrtd) / a;
        if (!a_tInterval.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!a_tInterval.surrounds(root))
                return false;
        }

        QVector3D hitPoint = a_ray.at(root);
        QVector3D outwardNormal = (hitPoint - m_center) / m_radius;
        a_outHit = HitRecord(hitPoint, m_material, root, a_ray, outwardNormal);

        return true;
    }

private:
    QVector3D m_center;
    double m_radius;
    MaterialPtr m_material;
};

#endif
