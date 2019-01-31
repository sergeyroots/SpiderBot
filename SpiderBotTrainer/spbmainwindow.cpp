#include "spbmainwindow.h"
#include "ui_spbmainwindow.h"

#include <QDebug>
#include <QStyle>

#include "testdata.h"
#include <math.h>
#include "cmdpositionitem.h"
#include "footitem.h"
#include "projectsavecontroller.h"
#include <QFileDialog>
#include <QVariant>
#include <QMessageBox>

void addWidgetToList(QListWidget *list, QWidget *widget) {
    QListWidgetItem *listWidgetItem = new QListWidgetItem(list);
    listWidgetItem->setSizeHint (widget->sizeHint());
    list->addItem(listWidgetItem);
    list->setItemWidget(listWidgetItem, widget);
}

spbMainWindow::spbMainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::spbMainWindow) {
    ui->setupUi(this);

    commandSeparator = ui->menuCommands->insertSeparator(ui->actionAdd_new_command);

    playTimer = new QTimer();

    footEditor = new SbmFootEditor(this);
    connect(ui->actionFootEditor, &QAction::triggered, this, &spbMainWindow::on_openFootEditor);
    connect(ui->actionOpen_project, &QAction::triggered, this, &spbMainWindow::on_openProject);
    connect(ui->actionSave_project, &QAction::triggered, this, &spbMainWindow::on_saveProject);
    connect(ui->actionSave_As_project, &QAction::triggered, this, &spbMainWindow::on_saveAsProject);
    connect(ui->actionExport_generated_data, &QAction::triggered, this, &spbMainWindow::on_export_generated_data);
    connect(ui->actionAdd_new_command, &QAction::triggered, this, &spbMainWindow::on_AddNewCommand);
    connect(ui->actionRemove_current_command, &QAction::triggered, this, &spbMainWindow::on_removeCurrentCommand);

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

    connect(ui->lwCmdStepList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_cmdStepListItemChanged);
    connect(ui->lwFootList, &QListWidget::currentItemChanged, this, &spbMainWindow::on_footListItemChanged);

    connect(viewer, &SbmViewer::onSelectFoot, this, &spbMainWindow::on_selectFoot);
    connect(sbmStepTime, &SbmStepTime::onChangedValue, this, &spbMainWindow::on_changeStepTime);

    connect(playTimer, &QTimer::timeout, this, &spbMainWindow::on_playTimerTimeout);

    projectData = new ProjectData();
    connect(projectData, &ProjectData::activeCommandChange, this, &spbMainWindow::activeCommandChange);
    ProjectDataCommand *cmd = createDefaultProjectCommand(&spiderBotSettings);
    projectData->addCommand(cmd);
}

spbMainWindow::~spbMainWindow() {
    playTimer->stop();
    delete ui;
}

void spbMainWindow::closeEvent(QCloseEvent *event) {

}

void spbMainWindow::on_openFootEditor() {
    footEditor->setFoot(&body, &foots[0]);
    footEditor->exec();
}

void spbMainWindow::on_openProject() {
    QString projectPathTmp = QFileDialog::getOpenFileName(this, "Open Project", "", "XML Files (*.xml)");
    if (!projectPathTmp.isEmpty()) {
        projectPath = projectPathTmp;        
        disconnect(projectData, &ProjectData::activeCommandChange, this, &spbMainWindow::activeCommandChange);
        foreach(QAction *action, ui->menuCommands->actions()) {
            if (action != ui->actionAdd_new_command && action != commandSeparator) {
                ui->menuCommands->removeAction(action);
            }
        }
        projectData = ProjectSaveController::load(projectPath);
        int cmdCount = projectData->getCommandCount();
        ProjectDataCommand *cmd;
        for (int i=0; i<cmdCount; ++i) {
            cmd = projectData->getCommand(i);
            QAction *action = new QAction (QString(cmd->getCommandName()).append(" (0x").append(QString::number(cmd->getCommandCode(), 16)).append(")"));
            action->setData(QVariant::fromValue(cmd));
            connect(action, &QAction::triggered, this, [this, action](){
                ProjectDataCommand *cmd = qvariant_cast<ProjectDataCommand*>(action->data());
                projectData->setActiveCommand(cmd);
            });
            ui->menuCommands->insertAction(commandSeparator, action);
        }
        connect(projectData, &ProjectData::activeCommandChange, this, &spbMainWindow::activeCommandChange);
        projectData->setActiveCommand(projectData->getActiveCommandIndex());
    }
}

void spbMainWindow::on_saveProject() {
    if (projectPath.isEmpty()) {
        projectPath = QFileDialog::getSaveFileName(this, "Save Project", "", "XML Files (*.xml)");
    }
    if (!projectPath.isEmpty()) {
        ProjectSaveController::save(projectPath, projectData);
    }
}

void spbMainWindow::on_saveAsProject() {
    QString projectPathTmp = QFileDialog::getSaveFileName(this, "Save Project", "", "XML Files (*.xml)");
    if (!projectPathTmp.isEmpty()) {
        projectPath = projectPathTmp;
        ProjectSaveController::save(projectPath, projectData);
    }
}

void spbMainWindow::on_export_generated_data() {
    on_bStop_clicked();
    QString outFilePath = QFileDialog::getSaveFileName(this, "Save data", "commands.h", "C Header(*.h)");
    if (!outFilePath.isEmpty()) {
        QFile templateFile("commands.h.t");
        generator->save(&templateFile, new QFile(outFilePath), projectData);
    }
}

void spbMainWindow::on_AddNewCommand() {
    ProjectDataCommand *cmd = createDefaultProjectCommand(&spiderBotSettings);
    projectData->addCommand(cmd);
    projectData->setActiveCommand(cmd);
}

void spbMainWindow::on_removeCurrentCommand() {
    ProjectDataCommand *cmd = projectData->getActiveCommand();
    foreach(QAction *action, ui->menuCommands->actions()) {
        ProjectDataCommand *cmdAction = qvariant_cast<ProjectDataCommand*>(action->data());
        if (cmdAction == cmd) {
            if (QMessageBox::information(this,
                                         "Delete command",
                                         QString("Delete current command \"").append(action->text()).append("\""),
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::No)
                    != QMessageBox::Yes) {
                return;
            }
            ui->menuCommands->removeAction(action);
            break;
        }
    }
    if (projectData->getCommandCount() < 2) {
        ProjectDataCommand *cmd = createDefaultProjectCommand(&spiderBotSettings);
        projectData->addCommand(cmd);
    }
    projectData->removeCommand(cmd);
}

void spbMainWindow::on_sTiming_sliderMoved(int position) {
    ui->lPosition->setText(QString("%1/%2").arg(QString::number(position), QString::number(ui->sTiming->maximum()+1)));
}

void spbMainWindow::on_bPalyPause_toggled(bool checked) {
    if (checked) {
        ui->bPalyPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->bPalyPause->setToolTip("Pause");        
        generator = new SbmCommandGenerator();
        generator->generate(projectData->getActiveCommand());
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

void spbMainWindow::on_sbInterval_valueChanged(double val) {
    double m = fmod(val, SBM_ITERATION_VAL);
    if (m < 0.001 || m > (SBM_ITERATION_VAL - 0.001)) {
        // update other steps
        double lastInterval = sbmStepTime->getTimeInterval();
        int stepsCnt = ui->lwCmdStepList->count();
        int stepCurrentIndex = ui->lwCmdStepList->currentRow();
        for (int i=0; i<stepsCnt; ++i) {
            if (i != stepCurrentIndex) {
                uint32_t stepTimeIterations = projectData->getActiveCommand()->getStep(i)->stepTimeIterations;
                projectData->getActiveCommand()->getStep(i)->stepTimeIterations = round(lastInterval * stepTimeIterations / val);
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
    int stepIndex = ui->lwCmdStepList->currentRow();
    sbmFootStepInfo_t *stepInfo = projectData->getActiveCommand()->getStep(stepIndex);
    if (stepInfo != nullptr) {
        sbmFootAngles_t *angles = stepInfo->angles;
        ui->lwFootList->clear();
        for (uint32_t i=0; i<angles->footCount; ++i) {
            FootItem *footItem = new FootItem(angles->segmentsCount[i], angles->angles[i]);
            connect(footItem, &FootItem::onFootAngleChanged, this, [this, i](uint32_t segmentCount, float *angles){
                Q_UNUSED(segmentCount)
                viewer->setFootAngles(i, angles);
                projectData->getActiveCommand()->getStep(ui->lwCmdStepList->currentRow())->angles->angles[i] = angles;
            });
            footItem->setFootIndex(static_cast<int>(i+1));
            footItem->setReadOnly(stepIndex == 0);
            addWidgetToList(ui->lwFootList, footItem);
            viewer->setFootAngles(i, angles->angles[i]);
        }
        ui->lwFootList->setMaximumHeight(static_cast<int>(31 * angles->footCount));
        sbmStepTime->setTimeInSteps(stepInfo->stepTimeIterations);
    }
}

void spbMainWindow::on_bCmdPosAdd_clicked() {
    sbmFootStepInfo_t *stepInfo = projectData->getActiveCommand()->getStep(ui->lwCmdStepList->currentRow());
    if (stepInfo != nullptr) {
        sbmFootStepInfo_t *newStepInfo = ProjectDataCommand::cloneStep(stepInfo);
        projectData->getActiveCommand()->addStep(newStepInfo);
        loadCommand(projectData->getActiveCommand());
        ui->lwCmdStepList->setCurrentRow(projectData->getActiveCommand()->getStepCount()-1);
    }
}

void spbMainWindow::on_bCmdPosDel_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow > 0) {
        ProjectDataCommand *cmd = projectData->getActiveCommand();
        cmd->removeStep(cRow);
        if (cRow >= cmd->getStepCount()) {
            --cRow;
        }
        loadCommand(cmd, cRow);
    }
}

void spbMainWindow::on_bCmdPosUp_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow > 1) {
        projectData->getActiveCommand()->swapSteps(cRow, cRow-1);
        loadCommand(projectData->getActiveCommand(), cRow-1);
    }
}

void spbMainWindow::on_bCmdPosDown_clicked() {
    int cRow = ui->lwCmdStepList->currentRow();
    if (cRow < (ui->lwCmdStepList->count() - 1)) {
        projectData->getActiveCommand()->swapSteps(cRow, cRow+1);
        loadCommand(projectData->getActiveCommand(), cRow+1);
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
    ProjectDataCommand *cmd = projectData->getActiveCommand();
    int cmdStepCnt = cmd->getStepCount();
    for (int i=1; i < cmdStepCnt; ++i) {
        totalSteps += cmd->getStep(i)->stepTimeIterations;
    }
    ui->lPosition->setText(QString("1/").append(QString::number(totalSteps)));
}

void spbMainWindow::loadCommand(ProjectDataCommand *cmd, int selectItem) {
    ui->lwCmdStepList->clear();
    uint32_t stepCount = cmd->getStepCount();
    ui->sbCCode->setValue(cmd->getCommandCode());
    ui->sbInterval->setValue(cmd->getInterval());
    ui->leCmdName->setText(cmd->getCommandName());
    CmdPositionItem *cmdStepItem;
    sbmFootStepInfo_t *stepInfo;
    for (uint32_t i=0; i<stepCount; ++i) {
        stepInfo = cmd->getStep(i);
        cmdStepItem = new CmdPositionItem(stepInfo);
        if (i ==0) {
            cmdStepItem->setName("Start position");
            cmdStepItem->setRemovable(false);
            cmdStepItem->setReadOnly(true);
        } else {
            cmdStepItem->setName(QString("Step").append(QString::number(i)));
        }
        addWidgetToList(ui->lwCmdStepList, cmdStepItem);
    }
    ui->lwCmdStepList->setCurrentRow(selectItem);
}

ProjectDataCommand *spbMainWindow::createDefaultProjectCommand(sbmSpiderBotSettings_t *settings) {
    ProjectDataCommand *cmd = ProjectDataCommand::createDefaultProjectCommand(settings);

    QAction *action = new QAction ("default (0x10)") ;
    action->setData(QVariant::fromValue(cmd));
    connect(action, &QAction::triggered, this, [this, action](){
        ProjectDataCommand *cmd = qvariant_cast<ProjectDataCommand*>(action->data());
        projectData->setActiveCommand(cmd);
    });
    ui->menuCommands->insertAction(commandSeparator, action);

    return cmd;
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
    sbmFootStepInfo_t *stepInfo = projectData->getActiveCommand()->getStep(ui->lwCmdStepList->currentRow());
    if (stepInfo != nullptr) {
        stepInfo->stepTimeIterations = timeInSteps;
        updateTotalStepsLabel();
    }
}

void spbMainWindow::on_leCmdName_textChanged(const QString &val) {
    ProjectDataCommand *activeCmd = projectData->getActiveCommand();
    activeCmd->setCommandName(val);
    ProjectDataCommand *actionCmd;
    foreach (QAction *action, ui->menuCommands->actions()) {
        actionCmd = qvariant_cast<ProjectDataCommand*>(action->data());
        if (actionCmd == activeCmd) {
            action->setText(QString(activeCmd->getCommandName()).append(" (0x").append(QString::number(activeCmd->getCommandCode(), 16)).append(")"));
        }
    }
}

void spbMainWindow::activeCommandChange(ProjectDataCommand *cmd, ProjectDataCommand *lastCmd) {
    loadCommand(cmd);
    ProjectDataCommand *actionCmd;
    foreach (QAction *action, ui->menuCommands->actions()) {
        QFont f = action->font();
        actionCmd = qvariant_cast<ProjectDataCommand*>(action->data());
        f.setBold((actionCmd == cmd) ? true : false);
        action->setFont(f);
    }
}

void spbMainWindow::on_sbCCode_valueChanged(int val) {
    ProjectDataCommand *activeCmd = projectData->getActiveCommand();
    activeCmd->setCommandCode(val);
    ProjectDataCommand *actionCmd;
    foreach (QAction *action, ui->menuCommands->actions()) {
        actionCmd = qvariant_cast<ProjectDataCommand*>(action->data());
        if (actionCmd == activeCmd) {
            action->setText(QString(activeCmd->getCommandName()).append(" (0x").append(QString::number(activeCmd->getCommandCode(), 16)).append(")"));
        }
    }
}
