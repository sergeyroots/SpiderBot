#include "sbmcommandgenerator.h"
#include <stdlib.h>
#include <string.h>

SbmCommandGenerator::SbmCommandGenerator(uint32_t stepCount):
        snapshotCount(0) {
    if (stepCount > 1000) {
        stepCount = 1000;
    }
    steps = static_cast<sbmFootStepInfo_t**>(malloc(sizeof(sbmFootStepInfo_t*) * stepCount));
    this->stepCount = stepCount;
}

SbmCommandGenerator::~SbmCommandGenerator() {
    freeGenerate();
    free(steps);
}

void SbmCommandGenerator::setStep(uint32_t index, sbmFootStepInfo_t *info) {
    if (index < stepCount) {
        steps[index] = info;
    }
}

SbmCommandGeneratorStatus_t SbmCommandGenerator::generate() {
    if (snapshotCount) {
        freeGenerate();
    }
    if (stepCount == 0) {
        return SBM_GENERATOR_ERR_NO_STEPS;
    }
    // malloc space
    uint32_t snapshotCnt = 1;
    for (uint32_t i=1; i<stepCount; ++i) {
        if (steps[i] == nullptr) {
            return SBM_GENERATOR_ERR_STEP_IS_NULL;
        }
        snapshotCnt += steps[i]->stepTimeIterations;
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
        preStep = steps[i-1];
        nextStep = steps[i];
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
    nextStep = steps[stepCount-1];
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
}

void SbmCommandGenerator::freeGenerate() {
    //...
    snapshotCount = 0;
}
