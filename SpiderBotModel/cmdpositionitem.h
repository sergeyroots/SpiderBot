#ifndef CMDPOSITIONITEM_H
#define CMDPOSITIONITEM_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>

#include "sbmcommandgenerator.h"
#include "sbmsettings.h"

class CmdPositionItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool removable READ isRemovable WRITE setRemovable)
public:
    explicit CmdPositionItem(sbmSpiderBotSettings_t *settings, QWidget *parent = nullptr);
    explicit CmdPositionItem(sbmFootAngles_t *angles, QWidget *parent = nullptr);
    bool isReadOnly() const;
    bool isRemovable() const;
    void setName(QString name);
    CmdPositionItem *clone(void);

private:
    bool m_readOnly;
    bool m_removable;
    QLabel *lName;
    QToolButton *bRemove;
    sbmFootAngles_t angles;
    void createUI(void);

signals:

public slots:
    void setReadOnly(bool readOnly);
    void setRemovable(bool removable);
    sbmFootAngles_t *getAngles(void);
};

#endif // CMDPOSITIONITEM_H
