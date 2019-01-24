#include "sbmfooteditor.h"
#include "ui_sbmfooteditor.h"

#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>

#include <string.h>
#include <stdlib.h>

SbmFootEditor::SbmFootEditor(QWidget *parent) :
        QDialog(parent), ui(new Ui::SbmFootEditor) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    foot = {0, nullptr, 0.f, 0.f, 0.f};
    footViewer = new SbmFootViewer(this);
    ui->hlMain->addWidget(footViewer);
    ui->hlMain->setStretch(1,1);

    ui->lwSegments->setMaximumHeight(10*20);
}

SbmFootEditor::~SbmFootEditor() {
    delete ui;
}

void SbmFootEditor::setFoot(sbmBody_t*body, sbmFoot_t *foot) {
    if (this->foot.segmentCount) {
        ui->lwSegments->clear();
        free(this->foot.segments);
    }
    this->foot.segments = reinterpret_cast<sbmFootElement_t*>(malloc(sizeof(sbmFootElement_t) * foot->segmentCount));
    memcpy(this->foot.segments, foot->segments, sizeof(sbmFootElement_t) * foot->segmentCount);
    this->foot.segmentCount = foot->segmentCount;

    for (uint32_t i=0; i< foot->segmentCount; i++) {
        ui->lwSegments->addItem(QString("Segment ").append(QString::number(i+1)));
    }

    ui->fX->setValue(static_cast<double>(foot->x));
    ui->fY->setValue(static_cast<double>(foot->y));
    ui->fZ->setValue(static_cast<double>(foot->z));

    footViewer->setFoot(&this->foot);
    footViewer->setBody(body);
}

sbmFoot_t *SbmFootEditor::getFoot() {
    return &foot;
}

void SbmFootEditor::on_lwSegments_currentRowChanged(int currentRow) {
    int cnt = static_cast<int>(foot.segmentCount);
    if (currentRow >= 0 && currentRow < cnt) {
        sbmFootElement_t *segment = &foot.segments[currentRow];

        ui->sbLength->setValue(segment->length);
        ui->sbSegRX->setValue(static_cast<double>(segment->rX));
        ui->sbSegRY->setValue(static_cast<double>(segment->rY));
        ui->sbSegRZ->setValue(static_cast<double>(segment->rZ));
        ui->sbSegAngleMin->setValue(static_cast<double>(segment->angleMin));
        ui->sbSegAngleMax->setValue(static_cast<double>(segment->angleMax));
        ui->sbSegAngleDefault->setValue(static_cast<double>(segment->angleDefault));

        QPalette p = ui->bColor->palette();
        p.setColor(QPalette::Button, segment->model.color);
        ui->bColor->setPalette(p);
        ui->bColor->setText(QString("RGB (%1, %2, %3)").arg(
                                QString::number(segment->model.color.red()),
                                QString::number(segment->model.color.green()),
                                QString::number(segment->model.color.blue())));
        ui->sbX->setValue(static_cast<double>(segment->model.x));
        ui->sbY->setValue(static_cast<double>(segment->model.y));
        ui->sbZ->setValue(static_cast<double>(segment->model.z));
        ui->sbRX->setValue(static_cast<double>(segment->model.rX));
        ui->sbRY->setValue(static_cast<double>(segment->model.rY));
        ui->sbRZ->setValue(static_cast<double>(segment->model.rZ));
        ui->leSTLPath->setText(segment->model.stlUrl.toLocalFile());
    }
}

void SbmFootEditor::on_bColor_clicked() {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
        QColor color = QColorDialog::getColor(ui->bColor->palette().color(QPalette::Button), this );
        QPalette p = ui->bColor->palette();
        p.setColor(QPalette::Button, color);
        ui->bColor->setPalette(p);
        ui->bColor->setText(QString("RGB (%1, %2, %3)").arg(
                                QString::number(color.red()),
                                QString::number(color.green()),
                                QString::number(color.blue())));
        foot.segments[segmentIndex].model.color = color;
        segmentModelColorApply(segmentIndex);
    }
}

void SbmFootEditor::segmentModelTranslateApply(uint32_t segmentIndex) {
    sbmFootElementModel_t *model = &foot.segments[segmentIndex].model;
    footViewer->segmentModelTranslate(segmentIndex, QVector3D(model->x, model->y, model->z));
}

void SbmFootEditor::segmentModelRotationApply(uint32_t segmentIndex) {
    sbmFootElementModel_t *model = &foot.segments[segmentIndex].model;
    footViewer->segmentModelRotation(segmentIndex, QVector3D(model->rX, model->rY, model->rZ));
}

void SbmFootEditor::segmentModelColorApply(uint32_t segmentIndex) {
    footViewer->segmentModelColor(segmentIndex, foot.segments[segmentIndex].model.color);
}

void SbmFootEditor::on_sbX_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.x = static_cast<float>(val);
       segmentModelTranslateApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbY_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.y = static_cast<float>(val);
       segmentModelTranslateApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbZ_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.z = static_cast<float>(val);
       segmentModelTranslateApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbRX_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.rX = static_cast<float>(val);
       segmentModelRotationApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbRY_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.rY = static_cast<float>(val);
       segmentModelRotationApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbRZ_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].model.rZ = static_cast<float>(val);
       segmentModelRotationApply(segmentIndex);
    }
}

QColor randColor() {
    return QColor::fromHslF(QRandomGenerator::global()->bounded(1.0), 0.9, 0.5);
}

void SbmFootEditor::on_bAddSegment_clicked() {
    sbmFootElement_t *newElements = reinterpret_cast<sbmFootElement_t*>(malloc(sizeof(sbmFootElement_t) * (foot.segmentCount + 1)));
    memcpy(newElements, foot.segments, sizeof(sbmFootElement_t) * foot.segmentCount);
    free(foot.segments);
    // copy new element from last
    if (foot.segmentCount) {
        uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
        if (segmentIndex < foot.segmentCount) {
            memcpy(&newElements[foot.segmentCount], &newElements[segmentIndex], sizeof(sbmFootElement_t));
        } else {
            memcpy(&newElements[foot.segmentCount], &newElements[foot.segmentCount-1], sizeof(sbmFootElement_t));
        }
        sbmFootElementModel_t *model = &newElements[foot.segmentCount].model;
        model->x = model->x + 10;
        model->y = model->y + 10;
        model->color = randColor();
    }
    foot.segments = newElements;
    foot.segmentCount = foot.segmentCount + 1;
    footViewer->setFoot(&foot);
    ui->lwSegments->addItem(QString("Segment ").append(QString::number(foot.segmentCount)));
}

void SbmFootEditor::on_bModelPath_clicked() {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
        QString filePath = QFileDialog::getOpenFileName(this, "Choose segment model", foot.segments[segmentIndex].model.stlUrl.toLocalFile(), "*.stl");
        if (!filePath.isEmpty()) {
            foot.segments[segmentIndex].model.stlUrl = QUrl::fromLocalFile(filePath);
            ui->leSTLPath->setText(filePath);
            footViewer->setFoot(&foot);
        }
    }
}

void SbmFootEditor::on_bRemoveSegment_clicked() {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
        if (segmentIndex < (foot.segmentCount - 1)) {
            memcpy(&foot.segments[segmentIndex], &foot.segments[segmentIndex+1], sizeof(sbmFootElement_t) * (foot.segmentCount - segmentIndex - 1));
        }
        foot.segmentCount = foot.segmentCount - 1;
        delete ui->lwSegments->takeItem(static_cast<int>(segmentIndex));
        footViewer->setFoot(&foot);
    }
}

void SbmFootEditor::on_bOk_clicked() {
    accept();
}

void SbmFootEditor::on_bCancel_clicked() {
    reject();
}

void SbmFootEditor::on_fX_valueChanged(double val) {
    foot.x = static_cast<float>(val);
    footViewer->translateFoot(QVector3D(foot.x, foot.y, foot.z));
}

void SbmFootEditor::on_fY_valueChanged(double val) {
    foot.y = static_cast<float>(val);
    footViewer->translateFoot(QVector3D(foot.x, foot.y, foot.z));
}

void SbmFootEditor::on_fZ_valueChanged(double val) {
    foot.z = static_cast<float>(val);
    footViewer->translateFoot(QVector3D(foot.x, foot.y, foot.z));
}

void SbmFootEditor::segmentRotationApply(uint32_t segmentIndex) {
    sbmFootElement_t *segment = &foot.segments[segmentIndex];
    footViewer->segmentRotation(segmentIndex, QVector3D(segment->rX, segment->rY, segment->rZ));
}

void SbmFootEditor::on_sbSegRX_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].rX = static_cast<float>(val);
       segmentRotationApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbSegRY_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].rY = static_cast<float>(val);
       segmentRotationApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbSegRZ_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].rZ = static_cast<float>(val);
       segmentRotationApply(segmentIndex);
    }
}

void SbmFootEditor::on_sbSegAngleMin_valueChanged(double val) {

}

void SbmFootEditor::on_sbSegAngleMax_valueChanged(double val) {

}

void SbmFootEditor::on_sbSegAngleDefault_valueChanged(double val) {

}

void SbmFootEditor::on_sbLength_valueChanged(double val) {
    uint32_t segmentIndex = static_cast<uint32_t>(ui->lwSegments->currentRow());
    if (segmentIndex < foot.segmentCount) {
       foot.segments[segmentIndex].length = static_cast<float>(val);
       footViewer->segmentLength(segmentIndex, static_cast<float>(val));
    }
}
