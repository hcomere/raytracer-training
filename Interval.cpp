#include <Interval.h>

#include <Common.h>

Interval::Interval() : m_min(+infinity), m_max(-infinity) {}
Interval::Interval(double a_min, double a_max) : m_min(a_min), m_max(a_max) {}

bool Interval::contains(double x) const {
    return m_min <= x && x <= m_max;
}

bool Interval::surrounds(double x) const
{
    return m_min < x && x < m_max;
}

double Interval::clamp(double a_x) const
{
    if (a_x < m_min) return m_min;
    if (a_x > m_max) return m_max;
    return a_x;
}

const Interval Interval::empty(+infinity, -infinity);
const Interval Interval::universe(-infinity, +infinity);
