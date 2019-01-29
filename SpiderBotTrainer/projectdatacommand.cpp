#include "projectdatacommand.h"
#include <stdlib.h>
#include <QDebug>

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
    if (index < steps.count()) {
        return steps.at(index);
    } else {
        return nullptr;
    }
}

void ProjectDataCommand::removeStep(uint32_t index) {
    if (index < steps.count()) {
        steps.remove(index);
    }
}

void ProjectDataCommand::swapSteps(int32_t index1, int32_t index2) {
    sbmFootStepInfo_t *tmpStepInfo = steps.at(index1);
    steps[index1] = steps.at(index2);
    steps[index2] = tmpStepInfo;
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

void ProjectDataCommand::setCommandName(QString name) {
    commandName = name;
}

QString ProjectDataCommand::getCommandName() {
    return commandName;
}

sbmFootStepInfo_t *ProjectDataCommand::cloneStep(sbmFootStepInfo_t *srcInfo) {
    sbmFootStepInfo_t *newInfo = static_cast<sbmFootStepInfo_t*>(malloc(sizeof(sbmFootStepInfo_t)));
    newInfo->stepTimeIterations = srcInfo->stepTimeIterations;
    sbmFootAngles_t* srcAngles = srcInfo->angles;
    sbmFootAngles_t* newAngles = static_cast<sbmFootAngles_t*>(malloc(sizeof(sbmFootAngles_t)));
    newInfo->angles = newAngles;
    uint32_t footCount = srcAngles->footCount;
    newAngles->footCount = footCount;
    newAngles->segmentsCount = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * footCount));
    newAngles->angles = static_cast<float**>(malloc(sizeof(float*) * footCount));
    uint32_t segmentCount;
    for (uint32_t i=0; i<footCount; ++i) {
        segmentCount = srcAngles->segmentsCount[i];
        newAngles->segmentsCount[i] = segmentCount;
        float *angles = static_cast<float*>(malloc(sizeof(float) * segmentCount));
        newAngles->angles[i] = angles;
        for (uint32_t j=0; j<segmentCount; ++j) {
            angles[j] = srcAngles->angles[i][j];
        }
    }
    return newInfo;
}

ProjectDataCommand* ProjectDataCommand::createDefaultProjectCommand(sbmSpiderBotSettings_t *settings) {
    sbmFootStepInfo_t *stepInfo = static_cast<sbmFootStepInfo_t*>(malloc(sizeof(sbmFootStepInfo_t)));
    stepInfo->stepTimeIterations = 100;
    stepInfo->angles = static_cast<sbmFootAngles_t*>(malloc(sizeof(sbmFootAngles_t)));
    stepInfo->angles->footCount = settings->footCount;
    stepInfo->angles->segmentsCount = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * settings->footCount));
    stepInfo->angles->angles = static_cast<float**>(malloc(sizeof(float*) * settings->footCount));
    uint32_t segmentCount;
    for (uint32_t i=0; i<settings->footCount; ++i) {
        segmentCount = settings->foots[i].segmentCount;
        stepInfo->angles->segmentsCount[i] = segmentCount;
        float *angles = static_cast<float*>(malloc(sizeof(float) * segmentCount));
        stepInfo->angles->angles[i] = angles;
        for (uint32_t j=0; j<segmentCount; ++j) {
            angles[j] = settings->foots[i].segments[j].angleDefault;
        }
    }

    ProjectDataCommand *cmd = new ProjectDataCommand(0x10, SBM_ITERATION_VAL*5);
    cmd->setCommandName("default");
    cmd->setCommandCode(0x10);
    cmd->setInterval(SBM_ITERATION_VAL * 5);
    cmd->addStep(stepInfo);

    return cmd;
}
