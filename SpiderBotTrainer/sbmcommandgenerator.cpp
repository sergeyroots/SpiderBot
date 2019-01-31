#include "sbmcommandgenerator.h"
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

SbmCommandGenerator::SbmCommandGenerator():
        snapshotCount(0) {
}

SbmCommandGenerator::~SbmCommandGenerator() {
    freeGenerate();
//    free(steps);
}

SbmCommandGeneratorStatus_t SbmCommandGenerator::generate(ProjectDataCommand *data) {
    if (snapshotCount) {
        freeGenerate();
    }
    uint32_t stepCount = data->getStepCount();
    if (stepCount == 0) {
        return SBM_GENERATOR_ERR_NO_STEPS;
    }
    // malloc space
    uint32_t snapshotCnt = 1;
    for (uint32_t i=1; i<stepCount; ++i) {
        if (data->getStep(i) == nullptr) {
            return SBM_GENERATOR_ERR_STEP_IS_NULL;
        }
        snapshotCnt += data->getStep(i)->stepTimeIterations;
    }
    snapshots = static_cast<sbmFootAngles_t*>(malloc(sizeof(sbmFootAngles_t) * snapshotCnt));
    if (snapshots == nullptr) {
        return SBM_GENERATOR_ERR_MALLOC;
    }
    snapshotCount = snapshotCnt;

    // generate
    sbmFootStepInfo_t *preStep;
    sbmFootStepInfo_t *nextStep;
    uint32_t iterations, footCount, footIndex, segmentCount, segmentIndex;
    uint32_t *segmentsCount;
    float preAngle, nextAngle;
    float *footAngles;
    sbmFootAngles_t* snapshot = &snapshots[0];
    for (uint32_t i=1; i<stepCount; ++i) {
        preStep = data->getStep(i-1);
        nextStep = data->getStep(i);
        footCount = nextStep->angles->footCount;
        iterations = nextStep->stepTimeIterations;
        for (uint32_t ii = 0; ii < iterations; ++ii) {
            segmentsCount = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * footCount));
            memcpy(segmentsCount, nextStep->angles->segmentsCount, sizeof(uint32_t) * footCount);
            snapshot->footCount = footCount;
            snapshot->segmentsCount = segmentsCount;
            snapshot->angles = static_cast<float**>(malloc(sizeof(float*) * snapshot->footCount));
            for(footIndex=0; footIndex<footCount; ++footIndex) {
                segmentCount = segmentsCount[footIndex];
                footAngles = static_cast<float*>(malloc(sizeof(float) * segmentCount));
                snapshot->angles[footIndex] = footAngles;
                for(segmentIndex=0; segmentIndex<segmentCount; ++segmentIndex) {
                    preAngle = preStep->angles->angles[footIndex][segmentIndex];
                    nextAngle = nextStep->angles->angles[footIndex][segmentIndex];
                    footAngles[segmentIndex] = preAngle + (nextAngle-preAngle)*ii/iterations;
                }
            }
            snapshot++;
        }
    }

    // last snapshot
    nextStep = data->getStep(stepCount-1);
    footCount = nextStep->angles->footCount;
    segmentsCount = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * footCount));
    memcpy(segmentsCount, nextStep->angles->segmentsCount, sizeof(uint32_t) * footCount);
    snapshot->footCount = footCount;
    snapshot->segmentsCount = segmentsCount;
    snapshot->angles = static_cast<float**>(malloc(sizeof(float*) * snapshot->footCount));
    for(footIndex=0; footIndex<footCount; ++footIndex) {
        segmentCount = segmentsCount[footIndex];
        footAngles = static_cast<float*>(malloc(sizeof(float) * segmentCount));
        snapshot->angles[footIndex] = footAngles;
        for(segmentIndex=0; segmentIndex<segmentCount; ++segmentIndex) {
            footAngles[segmentIndex] = nextStep->angles->angles[footIndex][segmentIndex];
        }
    }

    return SBM_GENERATOR_OK;
}

uint32_t SbmCommandGenerator::getSnapshotCount() {
    return snapshotCount;
}

sbmFootAngles_t *SbmCommandGenerator::getSnapshot(uint32_t index) {
    if (index < snapshotCount) {
        return &snapshots[index];
    }
    return nullptr;
}

SbmCommandGeneratorStatus_t SbmCommandGenerator::save(QFile *fileTemplate, QFile *outFile, ProjectData *data) {
    if (fileTemplate == nullptr || !fileTemplate->exists() || !fileTemplate->fileName().endsWith(".h.t")) {
        return SBM_GENERATOR_ERR_TEMPLATE;
    }
    if (!fileTemplate->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return SBM_GENERATOR_ERR_IO;
    }
    if (!outFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        return SBM_GENERATOR_ERR_IO;
    }
    QTextStream in(fileTemplate);
    QString templateString = in.readAll();
    fileTemplate->close();

    int cmdCount = data->getCommandCount();
    if (cmdCount == 0) {
        return SBM_GENERATOR_ERR_NO_STEPS;
    }

    uint32_t footCount = 0;
    uint32_t segmentsCount = 0;
    ProjectDataCommand *cmd;
    sbmFootAngles_t *angles;
    uint32_t snapshotCount;
    QString cmdCode;
    QString cmdDataArrayOut;
    QString cmdArrayOut;
    for (int iCmd=0; iCmd<cmdCount; ++iCmd) {
        cmd = data->getCommand(iCmd);
        cmdCode = QString("0x").append(QString::number(cmd->getCommandCode(), 16));
        QString cmdDataOut = QString("// Command: ").append(cmd->getCommandName()).append("\nstatic const uint8_t cmd_data_").append(cmdCode).append("[] PROGMEM = {\n");
        generate(cmd);
        snapshotCount = getSnapshotCount();
        for (uint32_t iSns=0; iSns<snapshotCount; ++iSns) {
            cmdDataOut.append("\t");
            angles = getSnapshot(iSns);
            footCount = angles->footCount;
            for (uint32_t iFoot=0; iFoot<footCount; ++iFoot) {
                cmdDataOut.append("/*").append(QString::number(iFoot+1)).append("*/");
                segmentsCount = angles->segmentsCount[iFoot];
                for (uint32_t iSeg=0; iSeg<segmentsCount; ++iSeg) {
                    cmdDataOut.append("CMD_A(").append(QString::number((int)round(angles->angles[iFoot][iSeg]))).append("), ");
                }
            }
            cmdDataOut.append("\n");
        }
        cmdDataOut.append("};\n\n");
        cmdDataArrayOut.append(cmdDataOut);
        cmdArrayOut.append("\t{").
                append(cmdCode).
                append(", cmd_data_").
                append(cmdCode).
                append(", ").append(QString::number(snapshotCount * footCount * segmentsCount)).append(", ").
                append(QString::number((int)round(cmd->getInterval() / SBM_ITERATION_VAL))).
                append("},\n");
    }
    QString servoArrayOut;
    for (uint32_t iFoot=0; iFoot<footCount; ++iFoot) {
        servoArrayOut.append("\t");
        for (uint32_t iSeg=0; iSeg<segmentsCount; ++iSeg) {
            servoArrayOut.append("SERVO_").append(QString::number(iFoot*segmentsCount + iSeg + 1)).append(", ");
        }
        servoArrayOut.append("\n");
    }
    templateString.replace("%%SERVO_ARRAY%%", servoArrayOut);
    templateString.replace("%%FOOT_COUNT%%", QString::number(footCount));
    templateString.replace("%%SEGMENT_COUNT%%", QString::number(segmentsCount));
    templateString.replace("%%CMD_DATA_ARRAYS%%", cmdDataArrayOut);
    templateString.replace("%%CMD_ARRAYS%%", cmdArrayOut);

    QTextStream out(outFile);
    out << templateString;

    outFile->close();
    return SBM_GENERATOR_OK;
}

void SbmCommandGenerator::freeGenerate() {
    //...
    snapshotCount = 0;
}
