#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include <memory>
#include <QVector3D>
#include <Common.h>
#include <Ray.h>

class Material;

class HitRecord
{
private:
    QVector3D m_point;
    QVector3D m_normal;
    MaterialPtr m_material;

    double m_t;
    bool m_frontFace;

public:
    HitRecord() = default;
    HitRecord(const QVector3D &a_point,
              MaterialPtr a_material,
              double a_t,
              const Ray& a_sourceRay,
              const QVector3D& a_outwardNormal);

public:
    const QVector3D& getPoint() const;
    const QVector3D& getNormal() const;
    MaterialPtr getMaterial() const;
    double getT() const;
    bool isFrontFace() const;
};

#endif
