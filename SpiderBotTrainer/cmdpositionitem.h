#ifndef CMDPOSITIONITEM_H
#define CMDPOSITIONITEM_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>

#include "sbmcommandgenerator.h"
#include "sbmsettings.h"
#include "projectdata.h"

class CmdPositionItem : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool removable READ isRemovable WRITE setRemovable)

public:
    explicit CmdPositionItem(sbmFootStepInfo_t *info, QWidget *parent = nullptr);
    bool isReadOnly() const;
    bool isRemovable() const;
    void setName(QString name);
    sbmFootStepInfo_t *getStepInfo(void);

private:
    sbmFootStepInfo_t *info;
    bool m_readOnly;
    bool m_removable;
    QLabel *lName;
//    QToolButton *bRemove;
    void createUI(void);

signals:

public slots:
    void setReadOnly(bool readOnly);
    void setRemovable(bool removable);
};

#endif // CMDPOSITIONITEM_H
