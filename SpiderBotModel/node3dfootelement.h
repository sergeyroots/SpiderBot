#ifndef NODE3DFOOTELEMENT_H
#define NODE3DFOOTELEMENT_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras>

#include "sbmsettings.h"
#include "qsectorplanemesh.h"

class Node3DFootElement : public Qt3DCore::QEntity {
    Q_PROPERTY(float angle READ getAbsAngle WRITE setAbsAngle)
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
    float getAbsAngle() const;
    float getRelAngle();
    void setRelAngle(float angle);

public slots:
    void setAbsAngle(float angle);

private:
    float length;
    float angleDefault;
    Qt3DCore::QEntity *model;
    Qt3DRender::QMesh *modelMesh;
    Qt3DExtras::QPhongMaterial *modelMaterial;
    Qt3DCore::QTransform *modelTransform;

    Qt3DCore::QEntity *vector;
    Qt3DExtras::QCylinderMesh *vectorMesh;
    Qt3DCore::QTransform *vectorTransform;
    QSectorPlaneMesh *sector;

    Qt3DCore::QTransform *transform;
    QQuaternion rot;
    float m_angle;
};

#endif // NODE3DFOOTELEMENT_H
