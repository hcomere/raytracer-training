#include <QVector3DUtils.h>
#include <CommonUtils.h>

namespace QVector3DUtils
{
    QVector3D lerp(const QVector3D& a_v1, const QVector3D& a_v2, double a_t)
    {
        return (1.0 - a_t) * a_v1 + a_t * a_v2;
    }

    QVector3D randomVector3D()
    {
        return QVector3D(CommonUtils::randomDouble(), CommonUtils::randomDouble(), CommonUtils::randomDouble());
    }

    QVector3D randomVector3D(double a_min, double a_max)
    {
        return QVector3D(CommonUtils::randomDouble(a_min, a_max), CommonUtils::randomDouble(a_min, a_max), CommonUtils::randomDouble(a_min, a_max));
    }

    QVector3D randomVector3DInUnitSphere()
    {
        while (true) {
            QVector3D p = randomVector3D(-1.0,1.0);
            if (p.lengthSquared() < 1.0)
                return p;
        }
    }

    QVector3D randomVector3DNormalized()
    {
        return randomVector3DInUnitSphere().normalized();
    }

    QVector3D randomVector3DOnHemisphere(const QVector3D& a_normal)
    {
        QVector3D onUnitSphere = randomVector3DNormalized();
        if (QVector3D::dotProduct(onUnitSphere, a_normal) > 0.0) // In the same hemisphere as the normal
            return onUnitSphere;
        else
            return -onUnitSphere;
    }

    QVector3D linearToGamma(QVector3D a_linearColor)
    {
        return QVector3D(sqrt(a_linearColor.x()), sqrt(a_linearColor.y()), sqrt(a_linearColor.z()));
    }

    bool vector3DIsNearZero(const QVector3D &a_vector)
    {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(a_vector[0]) < s) && (fabs(a_vector[1]) < s) && (fabs(a_vector[2]) < s);
    }

    QVector3D reflectVector3D(const QVector3D &a_vector, const QVector3D &a_normal)
    {
        return a_vector - 2.0 * QVector3D::dotProduct(a_vector,a_normal)*a_normal;
    }
}
