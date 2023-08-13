#include <common.h>
#include <random>

namespace common
{
    double degreesTo_Radians(double a_degrees) {
        return a_degrees * pi / 180.0;
    }

    QColor colorLerp(const QColor& a_color1, const QColor& a_color2, double a_t)
    {
        return QColor::fromRgbF(
            (1.0 - a_t) * a_color1.redF() + a_t * a_color2.redF(),
            (1.0 - a_t) * a_color1.greenF() + a_t * a_color2.greenF(),
            (1.0 - a_t) * a_color1.blueF() + a_t * a_color2.blueF()
            );
    }

    QColor colorMultScalar(const QColor& a_color, double a_scalar)
    {
        return QColor::fromRgbF(a_color.redF() * a_scalar, a_color.greenF() * a_scalar, a_color.blueF() * a_scalar);
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
}
