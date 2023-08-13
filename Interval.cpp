#include <Interval.h>

#include <common.h>

Interval::Interval() : m_min(+common::infinity), m_max(-common::infinity) {}
Interval::Interval(double a_min, double a_max) : m_min(a_min), m_max(a_max) {}
bool Interval::contains(double x) const {
        return m_min <= x && x <= m_max;
    }

bool Interval::surrounds(double x) const {
        return m_min < x && x < m_max;
    }

const Interval Interval::empty(+common::infinity, -common::infinity);
const Interval Interval::universe(-common::infinity, +common::infinity);
