#include <common.h>
#include <random>

namespace common
{
    double degreesTo_Radians(double a_degrees) {
        return a_degrees * pi / 180.0;
    }

    QVector3D lerp(const QVector3D& a_v1, const QVector3D& a_v2, double a_t)
    {
        return (1.0 - a_t) * a_v1 + a_t * a_v2;
    }

    double randomDouble()
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    double randomDouble(double a_min, double a_max)
    {
        // Returns a random real in [min,max).
        return a_min + (a_max-a_min)*randomDouble();
    }

    QVector3D randomVector3D()
    {
        return QVector3D(randomDouble(), randomDouble(), randomDouble());
    }

    QVector3D randomVector3D(double a_min, double a_max)
    {
        return QVector3D(randomDouble(a_min, a_max), randomDouble(a_min, a_max), randomDouble(a_min, a_max));
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
}
