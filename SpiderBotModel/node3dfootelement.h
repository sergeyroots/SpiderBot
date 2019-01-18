#ifndef NODE3DFOOTELEMENT_H
#define NODE3DFOOTELEMENT_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras>

#include "sbmsettings.h"

class Node3DFootElement : public Qt3DCore::QEntity {
public:
    Node3DFootElement(sbmFootElement_t *element);
    ~Node3DFootElement();

    Qt3DCore::QTransform *getTransform(void);
    void setRotation(QVector3D vector);
    QQuaternion getRotation(void);
    void setLength(float len);
    float getLength(void);

    void modelTranslate(QVector3D vector);
    void modelRotate(QVector3D vector);
    void modelColor(QColor modelColor);

    void setVectorEnabled(bool en);
    bool isVectorEnabled(void);
    void setModelEnable(bool en);

private:
    float length;
    Qt3DCore::QEntity *model;
    Qt3DRender::QMesh *modelMesh;
    Qt3DExtras::QPhongMaterial *modelMaterial;
    Qt3DCore::QTransform *modelTransform;

    Qt3DCore::QEntity *vector;
    Qt3DExtras::QCylinderMesh *vectorMesh;
     Qt3DCore::QTransform *vectorTransform;

    Qt3DCore::QTransform *transform;
    QQuaternion rot;
};

#endif // NODE3DFOOTELEMENT_H
