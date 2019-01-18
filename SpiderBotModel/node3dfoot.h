#ifndef NODE3DFOOT_H
#define NODE3DFOOT_H

#include <Qt3DCore/QEntity>
#include<Qt3DCore/QTransform>
//#include <Qt3DExtras>

#include "sbmsettings.h"
#include "node3dfootelement.h"

class Node3DFoot : public Qt3DCore::QEntity {
public:
    Node3DFoot(sbmFoot_t *foot);
    ~Node3DFoot();
    void translate(QVector3D vector);
    void segmentRotate(uint32_t segment, QVector3D vector);
    void segmentLength(uint32_t segment, float len);
    void segmentModelTranslate(uint32_t segment, QVector3D vector);
    void segmentModelRotate(uint32_t segment, QVector3D vector);
    void segmentModelColor(uint32_t segment, QColor color);
    void setVectorEnabled(bool en);
    bool isVectorEnabled(void);

private:
    Qt3DCore::QTransform* transform;
    uint32_t segmentCount;
    Node3DFootElement **segmentNodes;    
    void updatePositionLastSegments(uint32_t segment);
};

#endif // NODE3DFOOT_H
