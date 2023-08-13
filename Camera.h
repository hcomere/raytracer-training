#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QPixmap>
#include <QColor>
#include <QProgressBar>
#include <Hittable.h>
#include <HittableList.h>

#include <Ray.h>

class Camera
{
private:

    QImage m_image;
    int m_totalPixelCount;
    int m_computedPixels;
    bool m_isStarted;
    bool m_isFinished;
    int m_imageWidth;
    int m_imageHeight;

    int m_currentPixelY;
    int m_currentPixelX;

    HittableList m_world;

public:
    Camera();

public:
    QPixmap computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent);

private:

    QColor rayColor(const Ray& a_ray, const Hittable& a_world);
};

#endif // RAYTRACER_H
