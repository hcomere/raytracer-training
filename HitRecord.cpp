#include <HitRecord.h>

HitRecord::HitRecord(const QVector3D &a_point,
                     MaterialPtr a_material,
                     double a_t,
                     const Ray& a_sourceRay,
                     const QVector3D& a_outwardNormal)
    : m_point(a_point)
    , m_material(a_material)
    , m_t(a_t)
{
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    m_frontFace = QVector3D::dotProduct(a_sourceRay.getDirection(), a_outwardNormal) < 0;
    m_normal = m_frontFace ? a_outwardNormal : -a_outwardNormal;
}

const QVector3D& HitRecord::getPoint() const
{
    return m_point;
}

const QVector3D& HitRecord::getNormal() const
{
    return m_normal;
}

MaterialPtr HitRecord::getMaterial() const
{
    return m_material;
}

double HitRecord::getT() const
{
    return m_t;
}

bool HitRecord::isFrontFace() const
{
    return m_frontFace;
}
