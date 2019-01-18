#ifndef SBMFOOTVIEWER_H
#define SBMFOOTVIEWER_H

#include <QWidget>
#include <Qt3DCore/QEntity>
#include "sbmsettings.h"
#include "node3dfoot.h"

class SbmFootViewer : public QWidget {
    Q_OBJECT
public:
    explicit SbmFootViewer(QWidget *parent = nullptr);
    void setFoot(sbmFoot_t *foot);
    void setBody(sbmBody_t*body);
    void translateFoot(QVector3D vector);
    void segmentRotation(uint32_t segment,  QVector3D vector);
    void segmentLength(uint32_t segment,  float len);
    void segmentModelTranslate(uint32_t segment,  QVector3D vector);
    void segmentModelRotation(uint32_t segment,  QVector3D vector);
    void segmentModelColor(uint32_t segment,  QColor color);

private:
     QWidget *container ;
     Qt3DCore::QEntity *rootEntity;
     Qt3DRender::QCamera *camera;
     Node3DFoot *node3DFoot;
     Qt3DCore::QEntity *node3DBody;
     Qt3DRender::QMesh *bodyMesh;
     Qt3DExtras::QPhongMaterial *bodyMaterial;

//protected:
//    void paintEvent(QPaintEvent* event);

signals:

public slots:
};

#endif // SBMFOOTVIEWER_H
