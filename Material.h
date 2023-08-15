#ifndef MATERIAL_H
#define MATERIAL_H

#include <Ray.h>
#include <Hittable.h>
#include <QVector3D>
#include <common.h>

class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray &a_sourceRay, const HitRecord &a_hit, QVector3D &a_outAttenuatedColor, Ray &a_outScatteredRay) const = 0;
};

class LambertianMaterial : public Material {
public:
    LambertianMaterial(const QVector3D &a_albedo)
        : m_albedo(a_albedo)
    {}

    bool scatter(const Ray &a_sourceRay, const HitRecord &a_hit, QVector3D &a_outAttenuatedColor, Ray &a_outScatteredRay) const override
    {
        QVector3D scatterDirection = a_hit.m_normal + common::randomVector3DNormalized();

        // Catch degenerate scatter direction
        if (common::vector3DIsNearZero(scatterDirection))
            scatterDirection = a_hit.m_normal;

        a_outScatteredRay = Ray(a_hit.m_p, scatterDirection);
        a_outAttenuatedColor = m_albedo;
        return true;
    }

private:
    QVector3D m_albedo;
};

class MetalMaterial : public Material {
public:
    MetalMaterial(const QVector3D &a_albedo, double a_fuzzRadius)
        : m_albedo(a_albedo)
        , m_fuzzRadius(a_fuzzRadius)
    {}

    bool scatter(const Ray &a_sourceRay, const HitRecord &a_hit, QVector3D &a_outAttenuatedColor, Ray &a_outScatteredRay) const override
    {
        QVector3D reflected = common::reflectVector3D(a_sourceRay.getDirection().normalized(), a_hit.m_normal);
        a_outScatteredRay = Ray(a_hit.m_p, reflected + m_fuzzRadius * common::randomVector3DNormalized());
        a_outAttenuatedColor = m_albedo;
        return (QVector3D::dotProduct(a_outScatteredRay.getDirection(), a_hit.m_normal) > 0);
    }

private:
    QVector3D m_albedo;
    double m_fuzzRadius;
};

#endif
