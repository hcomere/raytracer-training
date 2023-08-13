#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <QColor>

namespace common
{
    static const double infinity = std::numeric_limits<double>::infinity();
    static const double pi = 3.1415926535897932385;

    double degreesTo_Radians(double a_degrees);
    QColor colorLerp(const QColor& a_color1, const QColor& a_color2, double a_t);
    QColor colorMultScalar(const QColor& a_color, double a_scalar);

    double randomDouble();
    double randomDouble(double a_min, double a_max);
}

#endif
