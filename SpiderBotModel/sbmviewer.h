#ifndef SBMVIEWER_H
#define SBMVIEWER_H

#include <QWidget>
#include <Qt3DCore/QEntity>
#include "sbmsettings.h"
#include "node3dfoot.h"

class SbmViewer : public QWidget
{
    Q_OBJECT
public:
    explicit SbmViewer(QWidget *parent = nullptr);
    void setSbmSettings(sbmSpiderBotSettings_t *settings);
    void setActiveFoot(uint32_t footIndex);
    void setFootAngles(uint32_t footIndex, float *angles);

private:
    QWidget *container ;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *camera;
    Node3DFoot **node3DFoots;
    Qt3DCore::QEntity *node3DBody;
    Qt3DRender::QMesh *bodyMesh;
    Qt3DExtras::QPhongMaterial *bodyMaterial;
    uint32_t footCount;

public slots:
    void onClickNode3DFoot(Qt3DRender::QPickEvent *event);

signals:
    void onSelectFoot(int32_t footIndex);
    void onChangeFootAngles(int32_t footIndex);
};

#endif // SBMVIEWER_H
