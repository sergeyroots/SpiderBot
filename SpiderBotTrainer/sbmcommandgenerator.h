#ifndef SBMCOMMANDGENERATOR_H
#define SBMCOMMANDGENERATOR_H

#include <stdint.h>
#include "projectdatacommand.h"

typedef enum {
    SBM_GENERATOR_OK = 0,
    SBM_GENERATOR_ERR_NO_STEPS,
    SBM_GENERATOR_ERR_STEP_IS_NULL,
    SBM_GENERATOR_ERR_MALLOC
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
    void freeGenerate(void);
};

#endif // SBMCOMMANDGENERATOR_H
