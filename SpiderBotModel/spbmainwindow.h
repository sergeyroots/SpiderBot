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
    void on_footListItemChanged(QListWidgetItem *item);
    void on_cmdStepListItemChanged(QListWidgetItem *item);

private:
    Ui::spbMainWindow *ui;
    SbmFootEditor *footEditor;
    SbmViewer *viewer;
};

#endif // SPBMAINWINDOW_H
