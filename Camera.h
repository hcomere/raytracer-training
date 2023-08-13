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
    bool m_isFinished;
    int m_imageWidth;
    int m_imageHeight;

    int m_currentPixelY;
    int m_currentPixelX;
    int m_currentPixelSampleCount;
    QVector3D m_currentPixelColor;

    HittableList m_world;

    int m_samplesPerPixel;

    QVector3D m_pixelDeltaU;
    QVector3D m_pixelDeltaV;
    QVector3D m_pixel00Loc;
    QVector3D m_cameraCenter;

public:
    Camera();

public:
    QPixmap computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent);

private:

    QVector3D rayColor(const Ray& a_ray, const Hittable& a_world);
    Ray getRay(int a_i, int a_j, bool a_random) const;
    QVector3D pixelSampleSquare() const;
};

#endif // RAYTRACER_H
