#include "sbmviewer.h"
#include <QVBoxLayout>
#include <Qt3DRender/QObjectPicker>
#include "qsbmcameracontroller.h"

SbmViewer::SbmViewer(QWidget *parent) : QWidget(parent) {
    node3DBody = nullptr;
    node3DFoots = nullptr;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    container = createWindowContainer(view, this);
    container->setMinimumSize(100, 100);

    Qt3DRender::QPickingSettings *pSettings = view->renderSettings()->pickingSettings();
    pSettings->setFaceOrientationPickingMode(Qt3DRender::QPickingSettings::FrontFace);
    pSettings->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    pSettings->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);

    container->setAttribute(Qt::WA_PaintOnScreen);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *l = new QVBoxLayout();
    l->setMargin(0);
    l->addWidget(container);
    setLayout(l);

    rootEntity = new Qt3DCore::QEntity;

    camera = view->camera();
//    camera->lens()->setPerspectiveProjection(60.0f, 16.0f/9.0f, 2.f, -1.0f);
    camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    camera->setUpVector(QVector3D(.0f, .0f, 1.0f));
    camera->setPosition(QVector3D(100, 600,  100.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));
    camera->setFarPlane(2000.f);
    //camera->rollAboutViewCenter(135);

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(camera);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.f);
    lightEntity->addComponent(light);

    Qt3DExtras::QSbmCameraController *camController = new Qt3DExtras::QSbmCameraController(rootEntity);
    camController->setCamera(camera);
    camController->setLinearSpeed( 500.0f );
    camController->setLookSpeed( 100.0f );
    view->setRootEntity(rootEntity);
    view->show();
}

void SbmViewer::setSbmSettings(sbmSpiderBotSettings_t *settings) {
    //create body
    if (node3DBody != nullptr) {
        node3DBody->removeComponent(bodyMesh);
        node3DBody->removeComponent(bodyMaterial);
        delete node3DBody;
    }
    node3DBody = new Qt3DCore::QEntity(rootEntity);
    bodyMesh = new Qt3DRender::QMesh();
    bodyMaterial = new Qt3DExtras::QPhongMaterial();

    node3DBody->addComponent(bodyMesh);
    node3DBody->addComponent(bodyMaterial);
    if (settings->body.stlUrl.isValid()) {
        bodyMaterial->setDiffuse(settings->body.color);
        //bodyMesh->setMeshName("Body");
        bodyMesh->setSource(settings->body.stlUrl);
    }

    // create foots
    footCount =  settings->footCount;
    if (node3DFoots != nullptr) {
        free(node3DFoots);
    }

    Qt3DRender::QObjectPicker  *picker;
    node3DFoots = static_cast<Node3DFoot**>(malloc(sizeof (Node3DFoot**) * footCount));
    for (uint32_t i=0; i<footCount; ++i) {
        node3DFoots[i] = new Node3DFoot(&settings->foots[i]);
        node3DFoots[i]->setVectorEnabled(false);
        node3DFoots[i]->setParent(rootEntity);
        picker = new Qt3DRender::QObjectPicker(node3DFoots[i]);
        picker->setHoverEnabled(false);
        picker->setEnabled(true);
        connect(picker, &Qt3DRender::QObjectPicker::pressed, this, &SbmViewer::onClickNode3DFoot);
        node3DFoots[i]->addComponent(picker);
    }
}

void SbmViewer::setActiveFoot(int32_t footIndex) {
    if (footIndex < static_cast<int32_t>(footCount)) {
        for (uint32_t i=0; i < footCount; ++i) {
            node3DFoots[i]->setVectorEnabled(false);
        }
        if (footIndex >= 0) {
            node3DFoots[footIndex]->setVectorEnabled(true);
        }
    }
}

void SbmViewer::setFootAngles(uint32_t footIndex, float *angles) {
    if (footIndex < footCount) {
        node3DFoots[footIndex]->setAngles(angles);
    }
}

void SbmViewer::onClickNode3DFoot(Qt3DRender::QPickEvent *event) {
    if (event->button() == Qt3DRender::QPickEvent::LeftButton) {
        int32_t currentIndex = -1;
        Node3DFoot *nodeFoot3D = dynamic_cast<Node3DFoot*>(sender()->parent());
        if (nodeFoot3D != nullptr) {
            for (uint32_t i=0; i < footCount; ++i) {
                if (node3DFoots[i] == nodeFoot3D) {
                    currentIndex = static_cast<int32_t>(i);
                }
                node3DFoots[i]->setVectorEnabled(false);
            }
            nodeFoot3D->setVectorEnabled(true);
        }
        emit onSelectFoot(currentIndex);
    }
}