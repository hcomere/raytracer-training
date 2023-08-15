#include <Camera.h>
#include <QVector3D>
#include <Ray.h>
#include <QDebug>
#include <QElapsedTimer>
#include <Sphere.h>
#include <Hittable.h>
#include <HittableList.h>
#include <common.h>

Camera::Camera(HWND a_hwnd, int a_windowWidth, int a_windowHeight)
    : m_isFinished(false)
    , m_colorBuffer(nullptr)
{
    m_hwnd = a_hwnd;
    m_hdc = GetDC(a_hwnd);

    setSize(a_windowWidth, a_windowHeight);

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

void Camera::computeImage(int a_feedbackInterval, bool& a_outIsFinished, float& a_outPercent)
{
    if(m_isFinished)
        return;

    QElapsedTimer timer;
    timer.start();

    int remainingTime = a_feedbackInterval - timer.elapsed();
    while (!m_isFinished && remainingTime > 0)
    {
        Ray ray = getRay(m_currentPixelX, m_currentPixelY, m_currentPixelSampleCount > 0);
        //Ray ray = getRay(m_currentPixelX, m_currentPixelY, true);

        QVector3D sampleColor = rayColor(ray, m_world, m_maxDepth);
        m_currentPixelColor += sampleColor;

        ++m_currentPixelSampleCount;
        if(m_currentPixelSampleCount >= m_samplesPerPixel)
        {
            m_currentPixelColor /= m_samplesPerPixel;

            // Move from linear color space to gamma color space more suitable to display on screen.
            m_currentPixelColor = common::linearToGamma(m_currentPixelColor);

            int startIndex = (m_windowHeight - 1 - m_currentPixelY) * m_windowWidth * 4 + m_currentPixelX * 4;
            m_colorBuffer[startIndex+2] = m_currentPixelColor.x() * 255.0;
            m_colorBuffer[startIndex+1] = m_currentPixelColor.y() * 255.0;
            m_colorBuffer[startIndex+0] = m_currentPixelColor.z() * 255.0;
            m_colorBuffer[startIndex+3] = 255.0;
            ++m_computedPixels;

            m_currentPixelSampleCount = 0;
            m_currentPixelColor = QVector3D(0,0,0);

            ++m_currentPixelX;
            if(m_currentPixelX >= m_windowWidth)
            {
                if(m_currentPixelY >= m_windowHeight-1)
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

    // Swap buffers
    // ============

    // Create a DC compatible with our window:
    HDC hBMPDC = CreateCompatibleDC(m_hdc);

    // Create a bitmap that is compatible with the window DC
    HBITMAP bmp = CreateCompatibleBitmap( m_hdc, m_windowWidth, m_windowHeight );

    // Setup the created bitmap and write our software buffer image into it:
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biClrImportant = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biWidth = m_windowWidth;
    bmi.bmiHeader.biHeight = m_windowHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;

    SetDIBits( hBMPDC, bmp, 0, m_windowHeight, m_colorBuffer, &bmi, DIB_RGB_COLORS );

    HGDIOBJ old = SelectObject( hBMPDC, bmp );

    // Shift the blitting position to make sure that our first pixel is in the lower left window corner.
    // (BitBlt is anchored top left, while the OpenGL SwapBuffer call is anchored bottom left). This can
    // be useful if the window client area is larger than the REDsdk window rendered into it. Without
    // this 'yshift' value, the hardware rendering differs from the software rendering:
    int yshift = 0;

    if(m_hwnd)
    {
        RECT rect;
        GetClientRect( m_hwnd, &rect );
        yshift = rect.bottom - rect.top - m_windowHeight;
    }

    // Blit the current rendering image into the window:
    BitBlt( m_hdc, 0, yshift, m_windowWidth, m_windowHeight, hBMPDC, 0, 0, SRCCOPY );

    SelectObject( hBMPDC, old );

    DeleteObject( bmp );
    DeleteDC( hBMPDC );
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

void Camera::setSize(int a_width, int a_height)
{
    if(m_colorBuffer != nullptr)
        delete [] m_colorBuffer;

    m_colorBuffer = new unsigned char [a_width * a_height * 4];
    m_windowWidth = a_width;
    m_windowHeight = a_height;

    double aspectRatio = (double)m_windowWidth/(double)m_windowHeight;

    // Camera

    double focalLength = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * aspectRatio;
    m_cameraCenter = QVector3D(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.

    QVector3D viewportU = QVector3D(viewportWidth, 0, 0);
    QVector3D viewportV = QVector3D(0, -viewportHeight, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.

    m_pixelDeltaU = viewportU / (double)m_windowWidth;
    m_pixelDeltaV = viewportV / (double)m_windowHeight;

    // Calculate the location of the upper left pixel.

    QVector3D viewportUpperLeft = m_cameraCenter - QVector3D(0, 0, focalLength) - viewportU * 0.5 - viewportV * 0.5;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    m_totalPixelCount = m_windowWidth * m_windowHeight;
    m_computedPixels = 0;

    m_isFinished = false;

    m_currentPixelY = 0;
    m_currentPixelX = 0;
    m_currentPixelSampleCount = 0;
    m_currentPixelColor = QVector3D(0, 0, 0);

    m_samplesPerPixel = 50;   // Count of random samples for each pixel
    m_maxDepth = 10;
}
