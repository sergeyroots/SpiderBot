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
    playTimer = new QTimer();

    footEditor = new SbmFootEditor(this);
    connect( ui->actionFootEditor, &QAction::triggered, this, &spbMainWindow::on_openFootEditor);

    viewer = new SbmViewer(this);
    viewer->setSbmSettings(&spiderBotSettings);
    ui->vlViewer->addWidget(viewer);

    //ui->bCmdPosAdd->setText("+");
    ui->bCmdPosDel->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->bCmdPosUp->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->bCmdPosDown->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));

    sbmStepTime = new SbmStepTime(ui->sbInterval->value());
    ui->vlCommandSettings->insertWidget(3, sbmStepTime);

    ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->bStepPlay->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->bStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->bRepeat->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lwCmdStepList->clear();
    CmdPositionItem *defaultCmdStep = new CmdPositionItem(&spiderBotSettings);
    defaultCmdStep->setName("Start position");
    defaultCmdStep->setRemovable(false);
    defaultCmdStep->setReadOnly(true);
    addWidgetToList(ui->lwCmdStepList, defaultCmdStep);
    connect(ui->lwCmdStepList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_cmdStepListItemChanged);
    connect(ui->lwFootList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_footListItemChanged);
    ui->lwCmdStepList->setCurrentRow(0);

    connect(viewer, &SbmViewer::onSelectFoot, this, &spbMainWindow::on_selectFoot);
    connect(sbmStepTime, &SbmStepTime::onChangedValue, this, &spbMainWindow::on_changeStepTime);

    connect(playTimer, &QTimer::timeout, this, &spbMainWindow::on_playTimerTimeout);
}

spbMainWindow::~spbMainWindow() {
    playTimer->stop();
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
        uint32_t stepCount = static_cast<uint32_t>(ui->lwCmdStepList->count());
        CmdPositionItem *cmdStep;
        generator = new SbmCommandGenerator(stepCount);
        for (uint32_t i=0; i<stepCount; ++i) {
            cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(ui->lwCmdStepList->item(i)));
            if (cmdStep != nullptr) {
                sbmFootStepInfo_t *info = static_cast<sbmFootStepInfo_t*>(malloc(sizeof(sbmFootStepInfo_t)));
                info->stepTimeIterations = cmdStep->getStepCount();
                info->angles = cmdStep->getAngles();
                generator->setStep(i, info);
            }
        }
        generator->generate();
        playIndex = 0;
        ui->sTiming->setValue(playIndex);
        ui->sTiming->setMaximum(generator->getSnapshotCount());
        viewer->setActiveFoot(-1);
        playTimer->start(round(ui->sbInterval->value())); //TODO round us!
    } else {
        ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->bPalyPause->setToolTip("Play");        
        playTimer->stop();
        viewer->setActiveFoot(ui->lwFootList->currentRow());
        delete generator;
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
        // update other steps
        double lastInterval = sbmStepTime->getTimeInterval();
        int stepsCnt = ui->lwCmdStepList->count();
        int stepCurrentIndex = ui->lwCmdStepList->currentRow();
        QListWidgetItem *item;
        CmdPositionItem *cmdStep;
        for (int i=0; i<stepsCnt; ++i) {
            if (i != stepCurrentIndex) {
                item = ui->lwCmdStepList->item(i);
                cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
                if (cmdStep != nullptr) {
                    cmdStep->setStepCount(round(lastInterval * cmdStep->getStepCount() / val));
                }
            }
        }
        // update current step
        sbmStepTime->setTimeInterval(val);
    }
}

void spbMainWindow::on_sbInterval_editingFinished() {
    ui->sbInterval->setValue(round(ui->sbInterval->value() / SBM_ITERATION_VAL) * SBM_ITERATION_VAL);
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
                Q_UNUSED(segmentCount)
                viewer->setFootAngles(i, angles);
            });
            footItem->setFootIndex(static_cast<int>(i+1));
            footItem->setReadOnly(cmdStep->isReadOnly());
            addWidgetToList(ui->lwFootList, footItem);
            viewer->setFootAngles(i, angles->angles[i]);
        }
        ui->lwFootList->setMaximumHeight(static_cast<int>(31 * angles->footCount));
        sbmStepTime->setTimeInSteps(cmdStep->getStepCount());
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
        updateTotalStepsLabel();
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
            updateTotalStepsLabel();
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

void spbMainWindow::on_bCCodeHex_clicked(bool checked) {
    if (checked) {
        ui->sbCCode->setDisplayIntegerBase(16);
        ui->sbCCode->setPrefix("0x");
    } else {
        ui->sbCCode->setDisplayIntegerBase(10);
        ui->sbCCode->setPrefix("");
    }
}

void spbMainWindow::on_playTimerTimeout() {
    if (generator != nullptr && playIndex < generator->getSnapshotCount()) {
        sbmFootAngles_t *angles = generator->getSnapshot(playIndex++);
        for(uint32_t i=0; i<angles->footCount; ++i) {
            viewer->setFootAngles(i, angles->angles[i]);
        }
        ui->sTiming->setValue(playIndex);
        ui->lPosition->setText(QString::number(playIndex+1).append("/").append(QString::number(generator->getSnapshotCount())));
    } else {
        if (ui->bRepeat->isChecked()) {
            playIndex = 0;
        } else {
            on_bPalyPause_toggled(false); //TODO:!
        }
    }
}

void spbMainWindow::updateTotalStepsLabel() {
    uint32_t totalSteps = 1;
    int cmdStepCnt = ui->lwCmdStepList->count();
    QListWidgetItem *item;
    for (int i=1; i < cmdStepCnt; ++i) {
        item = ui->lwCmdStepList->item(i);
        CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
        if (cmdStep != nullptr) {
            totalSteps += cmdStep->getStepCount();
        }
    }
    ui->lPosition->setText(QString("1/").append(QString::number(totalSteps)));
}

void spbMainWindow::on_footListItemChanged(QListWidgetItem *item, QListWidgetItem *itemPeriods) {
    int row = ui->lwFootList->row(item);
    FootItem *footItemPeriods = dynamic_cast<FootItem*>(ui->lwFootList->itemWidget(itemPeriods));
    if (footItemPeriods != nullptr) {
        footItemPeriods->setEditVisible(false);
    }
    viewer->setActiveFoot(row);
}

void spbMainWindow::on_selectFoot(int32_t footIndex) {
    if (footIndex < ui->lwFootList->count()) {
        ui->lwFootList->setCurrentRow(footIndex);
    }
}

void spbMainWindow::on_changeStepTime(uint32_t timeInSteps, double timeInterval) {
    QListWidgetItem *item = ui->lwCmdStepList->currentItem();
    if (item != nullptr) {
        CmdPositionItem *cmdStep = dynamic_cast<CmdPositionItem*>(ui->lwCmdStepList->itemWidget(item));
        if (cmdStep != nullptr) {
            cmdStep->setStepCount(timeInSteps);
            updateTotalStepsLabel();
        }
    }
}
