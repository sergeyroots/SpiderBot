#include "spbmainwindow.h"
#include "ui_spbmainwindow.h"

#include <QDebug>
#include <QStyle>

#include "testdata.h"
#include <math.h>
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

    lastIntervalVal = ui->sbInterval->value();

    //ui->bCmdPosAdd->setText("+");
    ui->bCmdPosDel->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->bCmdPosUp->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->bCmdPosDown->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));

    ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->bStepPlay->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->bStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->bRepeat->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lwCmdStepList->clear();
    CmdPositionItem *defaultCmdStep = new CmdPositionItem(&spiderBotSettings);
    defaultCmdStep->setRemovable(false);
    defaultCmdStep->setReadOnly(true);
    addWidgetToList(ui->lwCmdStepList, defaultCmdStep);
    connect(ui->lwCmdStepList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_cmdStepListItemChanged);
    connect(ui->lwFootList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_footListItemChanged);
    ui->lwCmdStepList->setCurrentRow(0);
    connect(viewer, &SbmViewer::onSelectFoot, this, &spbMainWindow::on_selectFoot);
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
        double stepTimeVal = ui->sbStepTimeVal->value();
        if (ui->bStepTime->isChecked()) { // ms
            ui->sbStepTimeVal->setSingleStep(val);
            ui->sbStepTimeVal->setMinimum(val);
            ui->sbStepTimeVal->setMaximum(100 * val);
            ui->sbStepTimeVal->setValue(round(stepTimeVal / val) * val);
        } else { // steps
            ui->sbStepTimeVal->setValue(round(stepTimeVal * lastIntervalVal / val));
        }
        lastIntervalVal = val;
    }
}

void spbMainWindow::on_sbInterval_editingFinished() {
    ui->sbInterval->setValue(round(ui->sbInterval->value() / SBM_ITERATION_VAL) * SBM_ITERATION_VAL);
}

void spbMainWindow::on_footListItemChanged(QListWidgetItem *item, QListWidgetItem *itemPeriods) {
    int row = ui->lwFootList->row(item);
    FootItem *footItemPeriods = dynamic_cast<FootItem*>(ui->lwFootList->itemWidget(itemPeriods));
    if (footItemPeriods != nullptr) {
        footItemPeriods->setEditVisible(false);
    }
    if (row >= 0) {
        viewer->setActiveFoot(static_cast<uint32_t>(row));
    }
}

void spbMainWindow::on_cmdStepListItemChanged(QListWidgetItem *item, QListWidgetItem *itemPeriods) {
    Q_UNUSED(itemPeriods);
    CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
    if (cmdStep != nullptr) {
        sbmFootAngles_t *angles = cmdStep->getAngles();
        ui->lwFootList->clear();
        for (uint32_t i=0; i<angles->footCount; ++i) {
            FootItem *footItem = new FootItem(angles->segmentsCount[i], angles->angles[i]);
            connect(footItem, &FootItem::onFootAngleChanged, this, [this, i](uint32_t segmentCount, float *angles){
                viewer->setFootAngles(i, angles);
            });
            footItem->setFootIndex(i+1);
            addWidgetToList(ui->lwFootList, footItem);
        }
        ui->lwFootList->setMaximumHeight(static_cast<int>(30 * angles->footCount)+2);
    }
}

void spbMainWindow::on_bCmdPosAdd_clicked() {
    CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(ui->lwCmdStepList->currentItem()));
     if (cmdStep != nullptr) {
        CmdPositionItem *newCmdStep = cmdStep->clone();
        newCmdStep->setRemovable(true);
        newCmdStep->setReadOnly(false);
        newCmdStep->setName(QString("step ").append(QString::number(ui->lwCmdStepList->count())));
        addWidgetToList(ui->lwCmdStepList, newCmdStep);
    }
}

void spbMainWindow::on_bCmdPosDel_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow > 0) {
        QListWidgetItem *item = ui->lwCmdStepList->currentItem();
        CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
        if (cmdStep != nullptr) {
            ui->lwCmdStepList->removeItemWidget(item);
            QListWidgetItem *item = ui->lwCmdStepList->takeItem(cRow);
            delete cmdStep;
            delete item;
        }
    }
}

void spbMainWindow::on_bCmdPosUp_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow > 1) {
        QListWidgetItem *item = ui->lwCmdStepList->item(cRow);
        CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
        if (cmdStep != nullptr) {
            ui->lwCmdStepList->removeItemWidget(item);
            item = ui->lwCmdStepList->takeItem(cRow);
            cRow--;
            CmdPositionItem *newCmdStep = cmdStep->clone();
            delete cmdStep;
            ui->lwCmdStepList->insertItem(cRow, item);
            ui->lwCmdStepList->setCurrentRow(cRow);
            ui->lwCmdStepList->setItemWidget(item, newCmdStep);
        }
    }
}

void spbMainWindow::on_bCmdPosDown_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow < (ui->lwCmdStepList->count() - 1)) {
        QListWidgetItem *item = ui->lwCmdStepList->item(cRow);
        CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
        if (cmdStep != nullptr) {
            ui->lwCmdStepList->removeItemWidget(item);
            item = ui->lwCmdStepList->takeItem(cRow);
            cRow++;
            CmdPositionItem *newCmdStep = cmdStep->clone();
            delete cmdStep;
            ui->lwCmdStepList->insertItem(cRow, item);
            ui->lwCmdStepList->setCurrentRow(cRow);
            ui->lwCmdStepList->setItemWidget(item, newCmdStep);
        }
    }
}

void spbMainWindow::on_selectFoot(int32_t footIndex) {
    if (footIndex < ui->lwFootList->count()) {
        ui->lwFootList->setCurrentRow(footIndex);
    }
}

void spbMainWindow::on_bCCodeHex_clicked(bool checked) {
    if (checked) {
        ui->sbCCode->setDisplayIntegerBase(16);
        ui->sbCCode->setPrefix("0x");
    } else {
        ui->sbCCode->setDisplayIntegerBase(10);
        ui->sbCCode->setPrefix("");
    }
}

void spbMainWindow::on_bStepTime_clicked(bool checked) {
    double interval = ui->sbInterval->value();
    double currentVal = ui->sbStepTimeVal->value();
    if (checked) { // ms
        ui->sbStepTimeVal->setSuffix(" ms");
        ui->sbStepTimeVal->setDecimals(3);
        ui->sbStepTimeVal->setSingleStep(interval);
        ui->sbStepTimeVal->setMinimum(interval);
        ui->sbStepTimeVal->setMaximum(100 * interval);
        ui->sbStepTimeVal->setValue(round(currentVal) * interval);
    } else { // steps
        ui->sbStepTimeVal->setSuffix(" steps");
        ui->sbStepTimeVal->setDecimals(0);
        ui->sbStepTimeVal->setSingleStep(1);
        ui->sbStepTimeVal->setMinimum(1);
        ui->sbStepTimeVal->setMaximum(100);
        ui->sbStepTimeVal->setValue(round(currentVal / interval));
    }
}

void spbMainWindow::on_sbStepTimeVal_editingFinished() {
    if (ui->bStepTime->isChecked()) { // ms
        double interval = ui->sbInterval->value();
        ui->sbInterval->setValue(round(ui->sbInterval->value() / interval) * interval);
    }
}
