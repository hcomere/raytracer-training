#include <World.h>

#include <Material.h>
#include <Sphere.h>

World::World()
{
    std::shared_ptr<Material> materialGround = std::make_shared<LambertianMaterial>(QVector3D(0.8, 0.8, 0.0));
    std::shared_ptr<Material> materialCenter = std::make_shared<LambertianMaterial>(QVector3D(0.7, 0.3, 0.3));
    std::shared_ptr<Material> materialLeft = std::make_shared<MetalMaterial>(QVector3D(0.8, 0.8, 0.8), 0.3);
    std::shared_ptr<Material> materialRight = std::make_shared<MetalMaterial>(QVector3D(0.8, 0.6, 0.2), 1.0);

    add(std::make_shared<Sphere>(QVector3D( 0.0, -100.5, -1.0), 100.0, materialGround));
    add(std::make_shared<Sphere>(QVector3D( 0.0,    0.0, -1.0),   0.5, materialCenter));
    add(std::make_shared<Sphere>(QVector3D(-1.0,    0.0, -1.0),   0.5, materialLeft));
    add(std::make_shared<Sphere>(QVector3D( 1.0,    0.0, -1.0),   0.5, materialRight));
}

World::World(HittablePtr a_object)
{
    add(a_object);
}

void World::clear()
{
    m_objects.clear();
}

void World::add(HittablePtr a_object)
{
    m_objects.push_back(a_object);
}

bool World::hit(const Ray& a_ray, const Interval& a_tInterval, HitRecord& a_outHit) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = a_tInterval.m_max;

    for (const auto& object : m_objects)
    {
        if (object->hit(a_ray, Interval(a_tInterval.m_min, closestSoFar), tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.getT();
            a_outHit = tempRec;
        }
    }

    return hitAnything;
}
