#include "qsectorplanemesh.h"

QSectorPlaneMesh::QSectorPlaneMesh()
{

}

float QSectorPlaneMesh::radius() const {
    return m_radius;
}

float QSectorPlaneMesh::angle() const {
    return m_angle;
}

void QSectorPlaneMesh::setRadius(float radius) {
    if (qFuzzyCompare(m_radius, radius)) {
        return;
    }

    m_radius = radius;
    emit radiusChanged(m_radius);
}

void QSectorPlaneMesh::setAngle(float angle) {
    if (qFuzzyCompare(m_angle, angle)) {
        return;
    }

    m_angle = angle;
    emit angleChanged(m_angle);
}
