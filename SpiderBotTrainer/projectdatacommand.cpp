#include "projectdatacommand.h"
#include <stdlib.h>

ProjectDataCommand::ProjectDataCommand(uint8_t commandCode, float interval) {
    this->commandCode = commandCode;
    this->interval = interval;
}

ProjectDataCommand::~ProjectDataCommand() {
}

void ProjectDataCommand::addStep(sbmFootStepInfo_t *info) {
    steps.append(info);
}

sbmFootStepInfo_t *ProjectDataCommand::getStep(uint32_t index) {
    return steps.at(index);
}

uint32_t ProjectDataCommand::getStepCount() {
    return steps.length();
}

uint8_t ProjectDataCommand::getCommandCode() {
    return commandCode;
}

void ProjectDataCommand::setCommandCode(uint8_t ccode) {
    commandCode = ccode;
}

float ProjectDataCommand::getInterval() {
    return interval;
}

void ProjectDataCommand::setInterval(float interval) {
    this->interval = interval;
}
