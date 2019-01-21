#ifndef SPBMAINWINDOW_H
#define SPBMAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "sbmfooteditor.h"
#include "sbmviewer.h"

namespace Ui {
    class spbMainWindow;
}

class spbMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit spbMainWindow(QWidget *parent = nullptr);
    ~spbMainWindow();

private slots:
    void on_openFootEditor();
    void on_sTiming_sliderMoved(int position);
    void on_bPalyPause_toggled(bool checked);
    void on_bStop_clicked();
    void on_sbInterval_valueChanged(double arg1);
    void on_sbInterval_editingFinished();
    void on_footListItemChanged(QListWidgetItem *item, QListWidgetItem *itemPeriods);
    void on_cmdStepListItemChanged(QListWidgetItem *item, QListWidgetItem *itemPeriods);
    void on_bCmdPosAdd_clicked();
    void on_bCmdPosDel_clicked();
    void on_bCmdPosUp_clicked();
    void on_bCmdPosDown_clicked();
    void on_selectFoot(int32_t footIndex);
    void on_bCCodeHex_clicked(bool checked);
    void on_bStepTime_clicked(bool checked);

    void on_sbStepTimeVal_editingFinished();

private:
    Ui::spbMainWindow *ui;
    SbmFootEditor *footEditor;
    SbmViewer *viewer;
    double lastIntervalVal;
};

#endif // SPBMAINWINDOW_H
