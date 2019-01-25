#ifndef PROJECTSAVECONTROLLER_H
#define PROJECTSAVECONTROLLER_H

#include <QSettings>
#include "projectdata.h"

class ProjectSaveController {
private:
public:
    ProjectSaveController();
    static void save(QString fineName, ProjectData *data);
    static ProjectData* load(QString fineName);
};

#endif // PROJECTSAVECONTROLLER_H
