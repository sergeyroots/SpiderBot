#ifndef SBMCOMMANDGENERATOR_H
#define SBMCOMMANDGENERATOR_H

#include <stdint.h>
#include <QFile>
#include "projectdata.h"

typedef enum {
    SBM_GENERATOR_OK = 0,
    SBM_GENERATOR_ERR_NO_STEPS,
    SBM_GENERATOR_ERR_STEP_IS_NULL,
    SBM_GENERATOR_ERR_MALLOC,
    SBM_GENERATOR_ERR_NO_SNAPSHOT,
    SBM_GENERATOR_ERR_TEMPLATE,
    SBM_GENERATOR_ERR_IO,
} SbmCommandGeneratorStatus_t;

class SbmCommandGenerator {

private:
    sbmFootAngles_t *snapshots;
    uint32_t snapshotCount;

public:
    SbmCommandGenerator();
    ~SbmCommandGenerator();
    //void setStep(uint32_t index, sbmFootStepInfo_t *info);
    SbmCommandGeneratorStatus_t generate(ProjectDataCommand *data);
    uint32_t getSnapshotCount(void);
    sbmFootAngles_t *getSnapshot(uint32_t index);
    SbmCommandGeneratorStatus_t save(QFile *fileTemplate, QFile *outFile, ProjectData *data);
    void freeGenerate(void);
};

#endif // SBMCOMMANDGENERATOR_H
