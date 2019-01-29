#ifndef SPBMAINWINDOW_H
#define SPBMAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTimer>
#include "sbmfooteditor.h"
#include "sbmviewer.h"
#include "sbmsteptime.h"
#include "sbmcommandgenerator.h"
#include "sbmsettings.h"
#include "projectdata.h"

namespace Ui {
    class spbMainWindow;
}

class spbMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit spbMainWindow(QWidget *parent = nullptr);
    ~spbMainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_openFootEditor();
    void on_openProject();
    void on_saveProject();
    void on_saveAsProject();
    void on_AddNewCommand();
    void on_removeCurrentCommand();
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
    void on_changeStepTime(uint32_t timeInSteps, double timeInterval);
    void on_bCCodeHex_clicked(bool checked);
    void on_playTimerTimeout(void);
    void on_leCmdName_textChanged(const QString &arg1);
    void activeCommandChange(ProjectDataCommand *cmd, ProjectDataCommand *lastCmd);
    void on_sbCCode_valueChanged(int arg1);

private:
    QAction *commandSeparator;
    QString projectPath;
    SbmSettings *settings;
    ProjectData *projectData;
    SbmCommandGenerator *generator;
    QTimer *playTimer;
    uint32_t playIndex;
    Ui::spbMainWindow *ui;
    SbmFootEditor *footEditor;
    SbmViewer *viewer;
    SbmStepTime *sbmStepTime;
    void updateTotalStepsLabel(void);
    void loadCommand(ProjectDataCommand *cmd, int selectItem = 0);
    ProjectDataCommand* createDefaultProjectCommand(sbmSpiderBotSettings_t *settings);
};

#endif // SPBMAINWINDOW_H
