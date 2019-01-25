#ifndef PROJECTDATACOMMAND_H
#define PROJECTDATACOMMAND_H

#include <stdint.h>
#include <QVector>

typedef struct {
    uint32_t footCount;
    uint32_t *segmentsCount;
    float **angles;
} sbmFootAngles_t;

typedef struct {
    sbmFootAngles_t *angles;
    uint32_t stepTimeIterations;
} sbmFootStepInfo_t;

class ProjectDataCommand {
private:
    QVector<sbmFootStepInfo_t*> steps;
    uint8_t commandCode;
    float interval;
public:
    ProjectDataCommand(uint8_t commandCode = 0x10, float interval = 16.384f);
    ~ProjectDataCommand();
    void addStep(sbmFootStepInfo_t *info);
    sbmFootStepInfo_t *getStep(uint32_t index);
    uint32_t getStepCount(void);
    uint8_t getCommandCode(void);
    void setCommandCode(uint8_t ccode);
    float getInterval(void);
    void setInterval(float interval);
};

#endif // PROJECTDATACOMMAND_H
