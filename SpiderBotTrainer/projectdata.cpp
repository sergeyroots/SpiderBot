#include "projectdata.h"

ProjectData::ProjectData() :
    m_activeCommand(-1) {
}

ProjectData::~ProjectData() {
}

void ProjectData::addCommand(ProjectDataCommand *cmd) {
    commands.append(cmd);
    if (m_activeCommand < 0) {
        m_activeCommand = 0;
        emit activeCommandChange(cmd, nullptr);
    }
}

bool ProjectData::removeCommand(ProjectDataCommand *cmd) {
    int index = commands.indexOf(cmd);
    if (index < 0 || commands.length() < 2) {
        return false;
    }
    if (index < (commands.length()-1)) {
        setActiveCommand(index + 1);
    } else {
        setActiveCommand(index - 1);
    }
    commands.remove(index);
    emit onRemoveCommand(cmd);
    return true;
}

int32_t ProjectData::getCommandCount() {
    return commands.count();
}

ProjectDataCommand *ProjectData::getCommand(int32_t index) {
    return commands.at(index);
}

int32_t ProjectData::getActiveCommandIndex(void) {
    return m_activeCommand;
}

ProjectDataCommand *ProjectData::getActiveCommand() {
    return commands.at(m_activeCommand);
}

void ProjectData::setActiveCommand(int32_t cmdIndex) {
    if (cmdIndex >= 0 && cmdIndex < commands.length()) {
        ProjectDataCommand *lastCmd = commands.at(m_activeCommand);
        m_activeCommand = cmdIndex;
        emit activeCommandChange(commands.at(m_activeCommand), lastCmd);
    }
}

void ProjectData::setActiveCommand(ProjectDataCommand *cmd) {
    int index = commands.indexOf(cmd);
    if (index >= 0) {
        ProjectDataCommand *lastCmd = commands.at(m_activeCommand);
        m_activeCommand = index;
        emit activeCommandChange(cmd, lastCmd);
    }
}
