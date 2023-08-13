#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
public:
    double m_min, m_max;

    Interval();
    Interval(double a_min, double a_max);

    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;

    static const Interval empty, universe;
};

#endif
