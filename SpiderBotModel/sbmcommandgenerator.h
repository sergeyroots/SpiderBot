#ifndef SBMCOMMANDGENERATOR_H
#define SBMCOMMANDGENERATOR_H

#include <stdint.h>

typedef struct {
    uint32_t footCount;
    uint32_t *segmentsCount;
    float **angles;
} sbmFootAngles_t;

class SbmCommandGenerator {

public:
    SbmCommandGenerator();
};

#endif // SBMCOMMANDGENERATOR_H
