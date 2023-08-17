#include <Common.h>
#include <CommonUtils.h>
#include <random>

namespace CommonUtils
{
    double degreesToRadians(double a_degrees) {
        return a_degrees * pi / 180.0;
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
