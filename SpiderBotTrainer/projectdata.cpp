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
    }
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

void ProjectData::setActiveCommand(int32_t activeCommand) {
    if (activeCommand > 0 && activeCommand < commands.length()) {
        m_activeCommand = activeCommand;
    }
}

void ProjectData::setActiveCommand(ProjectDataCommand *cmd) {
    int index = commands.indexOf(cmd);
    if (index >= 0) {
        m_activeCommand = index;
    }
}
