#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <QVector3D>
#include <memory>

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class World;
using WorldPtr = std::shared_ptr<World>;

class Hittable;
using HittablePtr = std::shared_ptr<Hittable>;
using HittablePtrVector = std::vector<HittablePtr>;

static const double infinity = std::numeric_limits<double>::infinity();
static const double pi = 3.1415926535897932385;

#endif
