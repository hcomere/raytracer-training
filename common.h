#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <QVector3D>

namespace common
{
    static const double infinity = std::numeric_limits<double>::infinity();
    static const double pi = 3.1415926535897932385;

    double degreesTo_Radians(double a_degrees);
    QVector3D lerp(const QVector3D& a_v1, const QVector3D& a_v2, double a_t);

    double randomDouble();
    double randomDouble(double a_min, double a_max);

    QVector3D randomVector3D();
    QVector3D randomVector3D(double a_min, double a_max);
    QVector3D randomVector3DNormalized();
    QVector3D randomVector3DOnHemisphere(const QVector3D& a_normal);
    bool vector3DIsNearZero(const QVector3D &a_vector);
    QVector3D reflectVector3D(const QVector3D &a_vector, const QVector3D &a_normal);

    QVector3D linearToGamma(QVector3D a_linearColor);
}

#endif
