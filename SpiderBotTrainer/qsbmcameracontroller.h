#ifndef QSBMCAMERACONTROLLER_H
#define QSBMCAMERACONTROLLER_H

#include <Qt3DExtras/qabstractcameracontroller.h>

QT_BEGIN_NAMESPACE

namespace Qt3DExtras {

//class QSbmCameraControllerPrivate;

class QSbmCameraController : public QAbstractCameraController
{
    Q_OBJECT
    Q_PROPERTY(float zoomInLimit READ zoomInLimit WRITE setZoomInLimit NOTIFY zoomInLimitChanged)

public:
    QSbmCameraController(Qt3DCore::QNode *parent = nullptr);
    ~QSbmCameraController();
    float zoomInLimit() const;

public slots:
    void setZoomInLimit(float zoomInLimit);

signals:
    void zoomInLimitChanged(float zoomInLimit);

private:
    float m_zoomInLimit;
    void moveCamera(const QAbstractCameraController::InputState &state, float dt) override;

};

}   // Qt3DExtras

QT_END_NAMESPACE

#endif // QSBMCAMERACONTROLLER_H
