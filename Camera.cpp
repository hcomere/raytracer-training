#include <Camera.h>
#include <QVector3D>
#include <Ray.h>
#include <QDebug>
#include <QElapsedTimer>
#include <Sphere.h>
#include <Hittable.h>
#include <HittableList.h>
#include <common.h>

Camera::Camera()
    : m_isStarted(false)
    , m_isFinished(false)
{

}

QColor Camera::rayColor(const Ray& a_ray, const Hittable& a_world)
{
    HitRecord hit;
    if(a_world.hit(a_ray, Interval(0.0, common::infinity), hit))
        return common::colorMultScalar(QColor::fromRgbF(hit.m_normal.x()+1, hit.m_normal.y()+1, hit.m_normal.z()+1), 0.5);

    QVector3D unitDirection = a_ray.getDirection().normalized();
    double t = 0.5*(unitDirection.y() + 1.0);

    QColor from = QColor::fromRgbF(1.0, 1.0, 1.0);
    QColor to = QColor::fromRgbF(0.5, 0.7, 1.0);

    return common::colorLerp(from, to, t);
}

QPixmap Camera::computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent)
{
    if(m_isFinished)
        return QPixmap::fromImage(m_image);

    // Image size and aspect ratio

    double desiredAspectRatio = 16.0 / 9.0;

    m_imageWidth = 400;
    m_imageHeight = (int)std::floor(m_imageWidth / desiredAspectRatio);
    if(m_imageHeight < 1) m_imageHeight = 1;

    double aspectRatio = (double)m_imageWidth/(double)m_imageHeight;

    // Camera

    double focalLength = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * aspectRatio;
    QVector3D cameraCenter = QVector3D(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.

    QVector3D viewportU = QVector3D(viewportWidth, 0, 0);
    QVector3D viewportV = QVector3D(0, -viewportHeight, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.

    QVector3D pixelDeltaU = viewportU / (double)m_imageWidth;
    QVector3D pixelDeltaV = viewportV / (double)m_imageHeight;

    // Calculate the location of the upper left pixel.

    QVector3D viewportUpperLeft = cameraCenter - QVector3D(0, 0, focalLength) - viewportU * 0.5 - viewportV * 0.5;
    QVector3D pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Hop hop hop

    if(!m_isStarted)
    {
        m_image = QImage(m_imageWidth, m_imageHeight, QImage::Format_RGB32);

        m_totalPixelCount = m_imageWidth * m_imageHeight;
        m_computedPixels = 0;

        m_isStarted = true;
        m_isFinished = false;

        m_currentPixelY = 0;
        m_currentPixelX = 0;

        // World

        m_world.add(std::make_shared<Sphere>(QVector3D(0,0,-1), 0.5));
        m_world.add(std::make_shared<Sphere>(QVector3D(0,-100.5,-1), 100));
    }

    QElapsedTimer timer;
    timer.start();

    int remainingTime = a_feedbackInterval - timer.elapsed();
    while (!m_isFinished && remainingTime > 0)
    {
        QVector3D pixel_center = pixel00Loc + (m_currentPixelX * pixelDeltaU) + (m_currentPixelY * pixelDeltaV);
        QVector3D ray_direction = pixel_center - cameraCenter;
        Ray ray(cameraCenter, ray_direction);

        QColor pixel_color = rayColor(ray, m_world);

        m_image.setPixelColor(m_currentPixelX, m_currentPixelY, pixel_color);

        ++m_computedPixels;

        ++m_currentPixelX;
        if(m_currentPixelX >= m_imageWidth)
        {
            if(m_currentPixelY >= m_imageHeight-1)
                m_isFinished = true;
            else
            {
                m_currentPixelX = 0;
                m_currentPixelY++;
            }
        }

        remainingTime = a_feedbackInterval - timer.elapsed();
    }

    a_outIsFinished = m_isFinished;
    a_outPercent = (float) m_computedPixels / (float) m_totalPixelCount * 100.0f;

    return QPixmap::fromImage(m_image);
}
