#include "sbmsettings.h"
#include <stdlib.h>
#include <string.h>
#include <QSettings>

#include <QDebug>

SbmSettings::SbmSettings() {

}

void SbmSettings::saveToFile(QString fileName, sbmSpiderBotSettings_t *settings) {
    QSettings *qsettings = new QSettings(fileName, QSettings::IniFormat);

    qsettings->beginGroup("SpiderBot");
    qsettings->setValue("t1", 42);
    qsettings->setValue("t2", "test");
    qsettings->setValue("t3", 123.456);
    qsettings->endGroup();

    qsettings->beginGroup("group");
        qsettings->beginGroup("subgroup");
            qsettings->setValue("t1", 42);
        qsettings->endGroup();
    qsettings->endGroup();


    qsettings->beginGroup("ArrayGroup");
    qsettings->beginWriteArray("arr");
    qsettings->setArrayIndex(0);
    qsettings->setValue("a", 42);
    qsettings->setArrayIndex(1);
    qsettings->setValue("a", 42);
    qsettings->setArrayIndex(2);
    qsettings->setValue("a", 42);
    qsettings->endArray();
    qsettings->endGroup();

    qDebug() << qsettings->childGroups().count();
}

sbmSpiderBotSettings_t *SbmSettings::loadFromFile(QString fileName) {
    return nullptr;
}

void SbmSettings::freeSettings(sbmSpiderBotSettings_t *settings) {

}
