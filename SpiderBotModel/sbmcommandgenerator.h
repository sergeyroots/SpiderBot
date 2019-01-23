#ifndef SBMCOMMANDGENERATOR_H
#define SBMCOMMANDGENERATOR_H

#include <stdint.h>

typedef struct {
    uint32_t footCount;
    uint32_t *segmentsCount;
    float **angles;
} sbmFootAngles_t;

typedef struct {
    sbmFootAngles_t *angles;
    uint32_t stepTimeIterations;
} sbmFootStepInfo_t;

typedef enum {
    SBM_GENERATOR_OK = 0,
    SBM_GENERATOR_ERR_NO_STEPS,
    SBM_GENERATOR_ERR_STEP_IS_NULL,
    SBM_GENERATOR_ERR_MALLOC
} SbmCommandGeneratorStatus_t;

class SbmCommandGenerator {

private:
    sbmFootStepInfo_t **steps;
    uint32_t stepCount;
    sbmFootAngles_t *snapshots;
    uint32_t snapshotCount;

public:
    SbmCommandGenerator(uint32_t stepCount);
    ~SbmCommandGenerator();
    void setStep(uint32_t index, sbmFootStepInfo_t *info);
    SbmCommandGeneratorStatus_t generate(void);
    uint32_t getSnapshotCount(void);
    sbmFootAngles_t *getSnapshot(uint32_t index);
    void freeGenerate(void);
};

#endif // SBMCOMMANDGENERATOR_H
