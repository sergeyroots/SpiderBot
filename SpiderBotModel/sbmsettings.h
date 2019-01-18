#ifndef SBMSETTINGS_H
#define SBMSETTINGS_H

#include <stdint.h>
#include <QUrl>
#include <QColor>

typedef struct {
    QUrl stlUrl;
    QColor color;
    float x, y, z;
    float rX, rY, rZ;
} sbmFootElementModel_t;

typedef struct {
    sbmFootElementModel_t  model ;
    float length;
    float rX, rY, rZ;
    //float ring; // 0 -360
    float angleMin, angleMax, angleDefault;
} sbmFootElement_t;

typedef struct {
    uint32_t segmentCount;
    sbmFootElement_t *segments;
    float x, y, z;
} sbmFoot_t;

typedef  struct {
    QColor color;
    QUrl stlUrl;
}  sbmBody_t;

typedef struct {
    uint32_t footCount;
    sbmBody_t body;
    sbmFoot_t *foots;
} sbmSpiderBotSettings_t;

class sbmSettings {

private:
//    sbmFoot_t *foots = nullptr;
//    uint32_t footCount = 0;

public:
    sbmSettings();
    void saveToFile(QString fileName, sbmSpiderBotSettings_t* settings);
    sbmSpiderBotSettings_t *loadFromFile(QString fileName);
    void freeSettings(sbmSpiderBotSettings_t* settings);

};

#endif // SBMSETTINGS_H
