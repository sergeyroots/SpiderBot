#ifndef QSECTORPLANEMESH_H
#define QSECTORPLANEMESH_H

#include <Qt3DRender/qgeometryrenderer.h>

class QSectorPlaneMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    float m_radius;
    float m_angle;

public:
    QSectorPlaneMesh();
    float radius() const;
    float angle() const;

public slots:
    void setRadius(float radius);
    void setAngle(float angle);

signals:
    void radiusChanged(float radius);
    void angleChanged(float angle);
};

#endif // QSECTORPLANEMESH_H
