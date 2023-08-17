#ifndef CAMERA_H
#define CAMERA_H

#include <Common.h>
#include <windows.h>

#include <QColor>
#include <QProgressBar>
#include <Hittable.h>

#include <Ray.h>

class Camera
{
private:

    HDC m_hdc;
    HWND m_hwnd;
    unsigned char * m_colorBuffer;
    int m_windowWidth;
    int m_windowHeight;

    int m_totalPixelCount;
    int m_computedPixels;
    bool m_isFinished;
    bool m_isStarted;

    int m_currentPixelY;
    int m_currentPixelX;
    int m_currentPixelSampleCount;
    QVector3D m_currentPixelColor;

    WorldPtr m_world;

    int m_samplesPerPixel;

    QVector3D m_pixelDeltaU;
    QVector3D m_pixelDeltaV;
    QVector3D m_pixel00Loc;
    QVector3D m_cameraCenter;

    int m_maxDepth;

public:
    Camera(HWND a_hwnd, int a_windowWidth, int a_windowHeight);

public:
    void computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent);
    void setSize(int a_width, int a_height);

private:

    QVector3D rayColor(const Ray& a_ray, WorldPtr a_world, int a_remainingDepth);
    Ray getRay(int a_i, int a_j, bool a_random) const;
    QVector3D pixelSampleSquare() const;
};

#endif // RAYTRACER_H
