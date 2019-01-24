#include "sbmsettings.h"
#include <stdlib.h>
#include <string.h>
#include <QSettings>

sbmSettings::sbmSettings() {

}

void sbmSettings::saveToFile(QString fileName, sbmSpiderBotSettings_t *settings) {
    QSettings *qsettings = new QSettings(fileName);

    qsettings->beginGroup("SpiderBot");
    qsettings->endGroup();
}

sbmSpiderBotSettings_t *sbmSettings::loadFromFile(QString fileName) {
    return nullptr;
}

void sbmSettings::freeSettings(sbmSpiderBotSettings_t *settings) {

}
