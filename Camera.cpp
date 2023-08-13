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
    : m_isFinished(false)
{
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
    m_cameraCenter = QVector3D(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.

    QVector3D viewportU = QVector3D(viewportWidth, 0, 0);
    QVector3D viewportV = QVector3D(0, -viewportHeight, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.

    m_pixelDeltaU = viewportU / (double)m_imageWidth;
    m_pixelDeltaV = viewportV / (double)m_imageHeight;

    // Calculate the location of the upper left pixel.

    QVector3D viewportUpperLeft = m_cameraCenter - QVector3D(0, 0, focalLength) - viewportU * 0.5 - viewportV * 0.5;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    // Image

    m_image = QImage(m_imageWidth, m_imageHeight, QImage::Format_RGB32);

    m_totalPixelCount = m_imageWidth * m_imageHeight;
    m_computedPixels = 0;

    m_isFinished = false;

    m_currentPixelY = 0;
    m_currentPixelX = 0;
    m_currentPixelSampleCount = 0;
    m_currentPixelColor = QVector3D(0, 0, 0);

    m_samplesPerPixel = 100;   // Count of random samples for each pixel
    m_maxDepth = 100;

    // World

    m_world.add(std::make_shared<Sphere>(QVector3D(0,0,-1), 0.5));
    m_world.add(std::make_shared<Sphere>(QVector3D(0,-100.5,-1), 100));
}

QVector3D Camera::rayColor(const Ray& a_ray, const Hittable& a_world, int a_remainingDepth)
{
    if(a_remainingDepth <= 0)
        return QVector3D(0,0,0);

    HitRecord hit;
    if(a_world.hit(a_ray, Interval(0.001, common::infinity), hit)) // 0.001 as min to avoid shadow acne issue
    {
        // non lambertian distribution
        //QVector3D bounceDirection = common::randomVector3DOnHemisphere(hit.m_normal);

        // lambertian distribution - more scattering around the normal
        QVector3D bounceDirection = hit.m_normal + common::randomVector3DNormalized();

        return 0.5 * rayColor(Ray(hit.m_p, bounceDirection), a_world, a_remainingDepth-1);
    }

    QVector3D unitDirection = a_ray.getDirection().normalized();
    double t = 0.5*(unitDirection.y() + 1.0);

    QVector3D from(1.0, 1.0, 1.0);
    QVector3D to(0.5, 0.7, 1.0);

    return common::lerp(from, to, t);
}

QPixmap Camera::computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent)
{
    if(m_isFinished)
        return QPixmap::fromImage(m_image);

    QElapsedTimer timer;
    timer.start();

    int remainingTime = a_feedbackInterval - timer.elapsed();
    while (!m_isFinished && remainingTime > 0)
    {
        //Ray ray = getRay(m_currentPixelX, m_currentPixelY, m_currentPixelSampleCount > 0);
        Ray ray = getRay(m_currentPixelX, m_currentPixelY, true);

        QVector3D sampleColor = rayColor(ray, m_world, m_maxDepth);
        m_currentPixelColor += sampleColor;

        ++m_currentPixelSampleCount;
        if(m_currentPixelSampleCount >= m_samplesPerPixel)
        {
            m_currentPixelColor /= m_samplesPerPixel;

            // Move from linear color space to gamma color space more suitable to display on screen.
            m_currentPixelColor = common::linearToGamma(m_currentPixelColor);

            m_image.setPixelColor(m_currentPixelX, m_currentPixelY, QColor::fromRgbF(m_currentPixelColor.x(), m_currentPixelColor.y(), m_currentPixelColor.z()));
            ++m_computedPixels;

            m_currentPixelSampleCount = 0;
            m_currentPixelColor = QVector3D(0,0,0);

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
        }

        remainingTime = a_feedbackInterval - timer.elapsed();
    }

    a_outIsFinished = m_isFinished;
    a_outPercent = (float) m_computedPixels / (float) m_totalPixelCount * 100.0f;

    return QPixmap::fromImage(m_image);
}

Ray Camera::getRay(int a_i, int a_j, bool a_random) const
{
    // Get a randomly sampled camera ray for the pixel at location i,j.

    QVector3D pixelCenter = m_pixel00Loc + (a_i * m_pixelDeltaU) + (a_j * m_pixelDeltaV);
    QVector3D pixelSample = pixelCenter;

    if(a_random)
        pixelSample += pixelSampleSquare();

    QVector3D rayOrigin = m_cameraCenter;
    QVector3D rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
}

QVector3D Camera::pixelSampleSquare() const
{
    // Returns a random point in the square surrounding a pixel at the origin.
    double px = -0.5 + common::randomDouble();
    double py = -0.5 + common::randomDouble();
    return (px * m_pixelDeltaU) + (py * m_pixelDeltaV);
}
