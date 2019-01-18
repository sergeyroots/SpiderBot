#include "spbmainwindow.h"
#include "ui_spbmainwindow.h"

#include <QDebug>
#include <QStyle>
#include <math.h>

#include "testdata.h"
#include "cmdpositionitem.h"
#include "footitem.h"

void addWidgetToList(QListWidget *list, QWidget *widget) {
    QListWidgetItem *listWidgetItem = new QListWidgetItem(list);
    listWidgetItem->setSizeHint (widget->sizeHint());
    list->addItem(listWidgetItem);
    list->setItemWidget(listWidgetItem, widget);
}

spbMainWindow::spbMainWindow(QWidget *parent) :
        QMainWindow(parent),  ui(new Ui::spbMainWindow) {
    ui->setupUi(this);

    footEditor = new SbmFootEditor(this);
    connect( ui->actionFootEditor, &QAction::triggered, this, &spbMainWindow::on_openFootEditor);

    viewer = new SbmViewer(this);
    viewer->setSbmSettings(&spiderBotSettings);
    ui->vlViewer->addWidget(viewer);

    ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->bStepPlay->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->bStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->bRepeat->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lwCmdStepList->clear();
    CmdPositionItem *defaultCmdStep = new CmdPositionItem(&spiderBotSettings);
    defaultCmdStep->setRemovable(false);
    addWidgetToList(ui->lwCmdStepList, defaultCmdStep);
    connect(ui->lwCmdStepList, &QListWidget::itemClicked, this, &spbMainWindow::on_cmdStepListItemChanged);
    connect(ui->lwFootList, &QListWidget::itemClicked, this, &spbMainWindow::on_footListItemChanged);

}

spbMainWindow::~spbMainWindow() {
    delete ui;
}

void spbMainWindow::on_openFootEditor() {
    footEditor->setFoot(&body, &foots[0]);
    footEditor->exec();
}

void spbMainWindow::on_sTiming_sliderMoved(int position) {
    ui->lPosition->setText(QString("%1/%2").arg(QString::number(position), QString::number(ui->sTiming->maximum()+1)));
}

void spbMainWindow::on_bPalyPause_toggled(bool checked) {
    if (checked) {
        ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->bPalyPause->setToolTip("Pause");
    } else {
        ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->bPalyPause->setToolTip("Play");
    }
}

void spbMainWindow::on_bStop_clicked() {
    ui->bPalyPause->setChecked(false);
    ui->sTiming->setValue(0);
}

#define SBM_ITERATION_VAL   16.384

void spbMainWindow::on_sbInterval_valueChanged(double val) {
    double m = fmod(val, SBM_ITERATION_VAL);
    if (m < 0.001 || m > (SBM_ITERATION_VAL - 0.001)) {
        qDebug() << val;
    }
}

void spbMainWindow::on_sbInterval_editingFinished() {
    ui->sbInterval->setValue(round(ui->sbInterval->value() / SBM_ITERATION_VAL) * SBM_ITERATION_VAL);
}

void spbMainWindow::on_footListItemChanged(QListWidgetItem *item) {
    qDebug() << "yyyy" << item->text();
}

void spbMainWindow::on_cmdStepListItemChanged(QListWidgetItem *item) {
    CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
    if (cmdStep != nullptr) {
        sbmFootAngles_t *angles = cmdStep->getAngles();
        ui->lwFootList->clear();
        for (uint32_t i=0; i<angles->footCount; ++i) {
            FootItem *footItem = new FootItem(angles->segmentsCount[i], angles->angles[i]);
            addWidgetToList(ui->lwFootList, footItem);
        }
        ui->lwFootList->setMaximumHeight(static_cast<int>(20 * angles->footCount));
    }
}
