#include "node3dfoot.h"

Node3DFoot::Node3DFoot(sbmFoot_t *foot) :
            transform(new Qt3DCore::QTransform()) {
    segmentCount = foot->segmentCount;
    segmentNodes = (Node3DFootElement**) malloc(sizeof (Node3DFootElement*) * segmentCount);
    for (uint32_t i = 0; i<segmentCount; ++i) {
        segmentNodes[i] = new Node3DFootElement(&foot->segments[i]);
        segmentNodes[i]->setParent(this);
    }
    if (segmentCount > 0) {
        updatePositionLastSegments(0);
    }
    transform->setTranslation(QVector3D(foot->x, foot->y, foot->z));
    this->addComponent(transform);
}

Node3DFoot::~Node3DFoot() {
    for (uint32_t i=0; i<segmentCount;++i) {
        //segmentNodes[i]->setParent(NULL);
        delete segmentNodes[i];
    }
    free(segmentNodes);
}

void Node3DFoot::translate(QVector3D vector) {
    transform->setTranslation(vector);
}

void Node3DFoot::updatePositionLastSegments(uint32_t segment) {
    Q_UNUSED(segment)
    QMatrix4x4 m;
    for (uint32_t i = 0; i< segmentCount; ++i) {
        m.rotate(segmentNodes[i]->getRotation());
        m.rotate(segmentNodes[i]->getRelAngle(), QVector3D(0,0,1));
        segmentNodes[i]->getTransform()->setMatrix(m);
       m.translate(QVector3D(0, segmentNodes[i]->getLength(), 0));
    }
}

void Node3DFoot::segmentRotate(uint32_t segment, QVector3D vector) {
    segmentNodes[segment]->setRotation(vector);
    updatePositionLastSegments(segment);
}

void Node3DFoot::segmentLength(uint32_t segment, float len) {
    segmentNodes[segment]->setLength(len);
    updatePositionLastSegments(segment);
}

void Node3DFoot::setAngles(float *angles) {
    for (uint32_t i = 0; i< segmentCount; ++i) {
        segmentNodes[i]->setAbsAngle(angles[i]);
    }
    updatePositionLastSegments(0);
}

void Node3DFoot::segmentModelTranslate(uint32_t segment, QVector3D vector) {
    segmentNodes[segment]->modelTranslate(vector) ;
}

void Node3DFoot::segmentModelRotate(uint32_t segment, QVector3D vector) {
    segmentNodes[segment]->modelRotate(vector) ;
}

void Node3DFoot::segmentModelColor(uint32_t segment, QColor color) {
    segmentNodes[segment]->modelColor(color) ;
}

void Node3DFoot::setVectorEnabled(bool en) {
    for (int i=0; i< segmentCount; ++i) {
        segmentNodes[i]->setVectorEnabled(en);
    }
}

bool Node3DFoot::isVectorEnabled() {
    return segmentCount && segmentNodes[0]->isVectorEnabled();
}
