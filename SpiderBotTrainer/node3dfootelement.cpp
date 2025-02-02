#include "node3dfootelement.h"

Node3DFootElement::Node3DFootElement(sbmFootElement_t *element) :
            model(new Qt3DCore::QEntity()),
            modelMesh(new Qt3DRender::QMesh()),
            modelMaterial(new Qt3DExtras::QPhongMaterial()),
            modelTransform(new Qt3DCore::QTransform()),
            vector(new Qt3DCore::QEntity()),
            vectorMesh(new Qt3DExtras::QCylinderMesh()),
            vectorTransform(new Qt3DCore::QTransform()),
            sector(new QSectorPlaneMesh()),
            transform(new Qt3DCore::QTransform()) {
    length = element->length;
    angleDefault = element->angleDefault;
    m_angle = angleDefault;
    //model
    if (element->model.stlUrl->isValid()) {
        model->setParent(this);

        modelMaterial->setDiffuse(element->model.color);
        model->addComponent(this->modelMaterial);

        //modelMesh->setMeshName("Foot1");
        modelMesh->setSource(*element->model.stlUrl);
        model->addComponent(this->modelMesh);

        modelTransform->setTranslation(QVector3D(element->model.x, element->model.y, element->model.z));
        modelTransform->setRotationX(element->model.rX);
        modelTransform->setRotationY(element->model.rY);
        modelTransform->setRotationZ(element->model.rZ);
        model->addComponent(this->modelTransform);
    }

    //vector1
    vector->setParent(this);
    Qt3DCore::QEntity *v1 = new Qt3DCore::QEntity(vector);
    vectorMesh->setLength(element->length);
    vectorMesh->setRadius(1.0f);
    v1->addComponent(vectorMesh);
    Qt3DExtras::QPhongMaterial *vMaterial = new Qt3DExtras::QPhongMaterial();
    vMaterial->setDiffuse(QColor("green"));
    v1->addComponent(vMaterial);
    vectorTransform->setTranslation(QVector3D(0,element->length/2,0));
    //v1Transform->setRotationY(90);
    v1->addComponent(vectorTransform);

    //vector2
    Qt3DCore::QEntity *v2 = new Qt3DCore::QEntity(vector);
    Qt3DExtras::QCylinderMesh *v2Mesh = new Qt3DExtras::QCylinderMesh();
    v2Mesh->setLength(100);
    v2Mesh->setRadius(.5f);
    v2->addComponent(v2Mesh);
    Qt3DExtras::QPhongMaterial *v2Material = new Qt3DExtras::QPhongMaterial();
    v2Material->setDiffuse(QColor("red"));
    v2->addComponent(v2Material);
    Qt3DCore::QTransform *v2Transform = new Qt3DCore::QTransform();
    //v2Transform->setTranslation(QVector3D(0,0,50));
    v2Transform->setRotationX(90);
    v2->addComponent(v2Transform);

    //sector
    Qt3DCore::QEntity *sec = new Qt3DCore::QEntity(vector);
    sector->setRadius(50);
    sector->setAngle(90);
    sec->addComponent(sector);
    Qt3DExtras::QPhongMaterial *secMaterial = new Qt3DExtras::QPhongMaterial();
    secMaterial->setDiffuse(QColor("red"));
    sec->addComponent(secMaterial);
    Qt3DCore::QTransform *secTransform = new Qt3DCore::QTransform();
    //secTransform->setRotationX(90);
    sec->addComponent(secTransform);

    //transform segment
    rot = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, element->rX) *
            QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, element->rY) *
            QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, element->rZ);
    transform->setRotation(rot);
    this->addComponent(transform);

}

Node3DFootElement::~Node3DFootElement() {
    model->removeComponent(this->modelMesh);
    model->removeComponent(this->modelTransform);
    model->removeComponent(this->modelMaterial);
    delete this->modelMesh;
    delete this->modelTransform;
    delete this->modelMaterial;
    delete model;
    delete vector;
}

Qt3DCore::QTransform *Node3DFootElement::getTransform() {
    return transform;
}

void Node3DFootElement::setRotation(QVector3D vector) {
    rot = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, vector.x()) *
            QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, vector.y()) *
            QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, vector.z());
}

QQuaternion Node3DFootElement::getRotation() {
    return rot;
}

void Node3DFootElement::setLength(float len) {
    length = len;
    vectorMesh->setLength(len);
    vectorTransform->setTranslation(QVector3D(0, len/2, 0));
}

float Node3DFootElement::getLength() {
    return length;
}

void Node3DFootElement::modelTranslate(QVector3D vector) {
    modelTransform->setTranslation(vector);
}

void Node3DFootElement::modelRotate(QVector3D vector) {
    modelTransform->setRotationX(vector.x());
    modelTransform->setRotationY(vector.y());
    modelTransform->setRotationZ(vector.z());
}

void Node3DFootElement::modelColor(QColor color) {
    modelMaterial->setDiffuse(color);
}

void Node3DFootElement::setVectorEnabled(bool en) {
    vector->setEnabled(en);
}

bool Node3DFootElement::isVectorEnabled() {
    return vector->isEnabled();
}

void Node3DFootElement::setModelEnable(bool en) {
    model->setEnabled(en);
}

float Node3DFootElement::getAbsAngle() const {
    return m_angle;
}

float Node3DFootElement::getRelAngle() {
    return m_angle - angleDefault;
}

void Node3DFootElement::setRelAngle(float angle) {
    m_angle = angleDefault + angle;
}

void Node3DFootElement::setAbsAngle(float angle){
    m_angle = angle;
}
