#ifndef CMDPOSITIONITEM_H
#define CMDPOSITIONITEM_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>

#include "sbmcommandgenerator.h"
#include "sbmsettings.h"
#include "projectdata.h"

class CmdPositionItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool removable READ isRemovable WRITE setRemovable)
    Q_PROPERTY(uint32_t stepCount READ getStepCount WRITE setStepCount)

public:
    explicit CmdPositionItem(sbmFootAngles_t *angles, QWidget *parent = nullptr);
    bool isReadOnly() const;
    bool isRemovable() const;
    void setName(QString name);
    sbmFootStepInfo_t *getStepInfo(void);
    CmdPositionItem *clone(void);
    uint32_t getStepCount() const;

private:
    bool m_readOnly;
    bool m_removable;
    uint32_t m_stepCount;
    QLabel *lName;
    QToolButton *bRemove;
    sbmFootAngles_t angles;
    void createUI(void);

signals:

public slots:
    void setReadOnly(bool readOnly);
    void setRemovable(bool removable);
    sbmFootAngles_t *getAngles(void);
    void setStepCount(uint32_t stepCount);
};

#endif // CMDPOSITIONITEM_H
