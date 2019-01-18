#ifndef SBMFOOTEDITOR_H
#define SBMFOOTEDITOR_H

#include <QDialog>
#include "sbmfootviewer.h"

namespace Ui {
    class SbmFootEditor;
}

class SbmFootEditor : public QDialog {
    Q_OBJECT

public:
    explicit SbmFootEditor(QWidget *parent = nullptr);
    ~SbmFootEditor();

    void setFoot(sbmBody_t*body, sbmFoot_t *foot);
    sbmFoot_t *getFoot(void);

private slots:
    void on_lwSegments_currentRowChanged(int currentRow);
    void on_bColor_clicked();
    void on_sbX_valueChanged(double val);
    void on_sbY_valueChanged(double val);
    void on_sbZ_valueChanged(double val);
    void on_sbRX_valueChanged(double arg1);
    void on_sbRY_valueChanged(double arg1);
    void on_sbRZ_valueChanged(double arg1);
    void on_bAddSegment_clicked();
    void on_bModelPath_clicked();
    void on_bRemoveSegment_clicked();
    void on_bOk_clicked();
    void on_bCancel_clicked();
    void on_fX_valueChanged(double arg1);
    void on_fY_valueChanged(double arg1);
    void on_fZ_valueChanged(double arg1);
    void on_sbSegRX_valueChanged(double arg1);
    void on_sbSegRY_valueChanged(double arg1);
    void on_sbSegRZ_valueChanged(double arg1);
    void on_sbSegAngleMin_valueChanged(double arg1);
    void on_sbSegAngleMax_valueChanged(double arg1);
    void on_sbSegAngleDefault_valueChanged(double arg1);

    void on_sbLength_valueChanged(double arg1);

private:
    Ui::SbmFootEditor *ui;
    SbmFootViewer *footViewer;
    sbmFoot_t foot;
    void segmentModelTranslateApply(uint32_t segmentIndex);
    void segmentModelRotationApply(uint32_t segmentIndex);
    void segmentRotationApply(uint32_t segmentIndex);
    void segmentModelColorApply(uint32_t segmentIndex);
};

#endif // SBMFOOTEDITOR_H
