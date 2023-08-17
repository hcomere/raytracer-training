#ifndef MATERIAL_H
#define MATERIAL_H

#include <Ray.h>
#include <Hittable.h>
#include <QVector3D>
#include <common.h>
#include <QVector3DUtils.h>

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
        QVector3D scatterDirection = a_hit.getNormal() + QVector3DUtils::randomVector3DNormalized();

        // Catch degenerate scatter direction
        if (QVector3DUtils::vector3DIsNearZero(scatterDirection))
            scatterDirection = a_hit.getNormal();

        a_outScatteredRay = Ray(a_hit.getPoint(), scatterDirection);
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
        QVector3D reflected = QVector3DUtils::reflectVector3D(a_sourceRay.getDirection().normalized(), a_hit.getNormal());
        a_outScatteredRay = Ray(a_hit.getPoint(), reflected + m_fuzzRadius * QVector3DUtils::randomVector3DNormalized());
        a_outAttenuatedColor = m_albedo;
        return (QVector3D::dotProduct(a_outScatteredRay.getDirection(), a_hit.getNormal()) > 0);
    }

private:
    QVector3D m_albedo;
    double m_fuzzRadius;
};

#endif
