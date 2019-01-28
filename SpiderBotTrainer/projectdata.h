#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include "projectdatacommand.h"
#include <QVector>
#include <QObject>

class ProjectData : public QObject {
    Q_OBJECT
    Q_PROPERTY(int32_t activeCommand READ getActiveCommandIndex WRITE setActiveCommand NOTIFY activeCommandChange)
private:
    QVector<ProjectDataCommand*> commands;
    int32_t m_activeCommand;
public:
    ProjectData();
    ~ProjectData();
    void addCommand(ProjectDataCommand *cmd);
    bool removeCommand(ProjectDataCommand *cmd);
    int32_t getCommandCount(void);
    ProjectDataCommand *getCommand(int32_t index);
    int32_t getActiveCommandIndex(void);
    ProjectDataCommand *getActiveCommand(void);
    void setActiveCommand(int32_t cmdIndex);
    void setActiveCommand(ProjectDataCommand *cmd);

signals:
    void activeCommandChange(ProjectDataCommand *cmd, ProjectDataCommand *lastCmd);
    void onRemoveCommand(ProjectDataCommand *cmd);
};

#endif // PROJECTDATA_H
