#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include "projectdatacommand.h"
#include <QVector>
#include <QObject>

class ProjectData : QObject {
    Q_OBJECT
    Q_PROPERTY(int32_t activeCommand READ getActiveCommandIndex WRITE setActiveCommand)
private:
    QVector<ProjectDataCommand*> commands;
    int32_t m_activeCommand;
public:
    ProjectData();
    ~ProjectData();
    void addCommand(ProjectDataCommand *cmd);
    int32_t getCommandCount(void);
    ProjectDataCommand *getCommand(int32_t index);
    int32_t getActiveCommandIndex(void);
    ProjectDataCommand *getActiveCommand(void);
    void setActiveCommand(int32_t activeCommand);
};

#endif // PROJECTDATA_H
