#ifndef QVECTOR3D_UTILS_H
#define QVECTOR3D_UTILS_H

#include <QVector3D>

namespace QVector3DUtils
{
    QVector3D lerp(const QVector3D& a_v1, const QVector3D& a_v2, double a_t);
    QVector3D randomVector3D();
    QVector3D randomVector3D(double a_min, double a_max);
    QVector3D randomVector3DNormalized();
    QVector3D randomVector3DOnHemisphere(const QVector3D& a_normal);
    bool vector3DIsNearZero(const QVector3D &a_vector);
    QVector3D reflectVector3D(const QVector3D &a_vector, const QVector3D &a_normal);
    QVector3D linearToGamma(QVector3D a_linearColor);
};

#endif
