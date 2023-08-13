#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray {
    public:
        Ray() {}
        Ray(const QVector3D & a_origin, const QVector3D& a_direction)
            : m_origin(a_origin), m_direction(a_direction)
        {}

        QVector3D getOrigin() const  { return m_origin; }
        QVector3D getDirection() const { return m_direction; }

        QVector3D at(double a_t) const {
            return m_origin + a_t*m_direction;
        }

    public:
        QVector3D m_origin;
        QVector3D m_direction;
};

#endif
