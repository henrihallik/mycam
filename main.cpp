#include <QCoreApplication>
#include <QImage>
#include <QImageWriter>
#include <libcamera/libcamera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/control_ids.h>
#include <libcamera/framebuffer_allocator.h>
#include <libcamera/request.h>

using namespace libcamera;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CameraManager manager;
    manager.start();

    Camera *camera = manager.get("camera0");
    if (!camera) {
        qDebug() << "Failed to access camera";
        return -1;
    }

    if (camera->acquire()) {
        qDebug() << "Failed to acquire camera";
        return -1;
    }

    /* Set camera configuration */
    camera->configuration()->validate();
    camera->configure();

    /* Create a frame buffer allocator */
    FrameBufferAllocator *allocator = new FrameBufferAllocator(camera);

    /* Create a request */
    Request *request = camera->createRequest();
    Buffer *buffer = allocator->allocate(0);
    request->addBuffer(buffer, FrameBuffer::PlaneY);
    request->controls().set(controls::SensorMode, 0);
    request->controls().set(controls::ExposureTime, 1000000); // 1 second exposure time

    /* Start the capture */
    camera->start();
    camera->queueRequest(request);
    request->wait();

    /* Save the captured image to a file */
    QImage image(buffer->planes()[0].data(),
                 buffer->metadata().size.width,
                 buffer->metadata().size.height,
                 QImage::Format_RGB888);
    QImageWriter writer("captured_image.jpg");
    writer.write(image);

    /* Clean up */
    camera->stop();
    allocator->free(buffer);
    delete allocator;
    camera->release();

    return a.exec();
}
